#ifndef WOW_TRANSLATOR_H
#define WOW_TRANSLATOR_H

#include <vector>
#include <unordered_set>
#include <set>
#include <string>
#include <algorithm>
#include "../tokenizer/tokenizer.h"
#include "../utils/poliz.h"

class Translator {
    std::vector<Token> tokens;

    std::vector<int> continuePos;
    std::vector<int> breakPos;

    struct Scope {
        enum TypeScope {
            SIMPLE, FUNC, CLASS
        };
        TypeScope type;
        std::string value; // for func, class
        int countCycle, countFunc;

        struct VarInfo {
            std::string name;

            const bool operator<(const VarInfo &other) const {
                return name < other.name;
            }
        };

        std::set<VarInfo> variables;

        struct FuncInfo {
            std::set<std::string> parentFunc;
        };

        std::map<std::string, FuncInfo> functions;

        struct ClassInfo {
            std::set<std::string> parentClasses;
        };

        std::map<std::string, ClassInfo> classes;

    };

    std::vector<Scope> scopes;
    std::vector<Scope> bigScopes;

    Poliz *poliz = nullptr;
    Poliz *nowPoliz = nullptr;

public:
    explicit Translator(std::vector<Token> &v) : tokens(v) {
        tokens.push_back(Token(Token::ENDMARKER, tokens.back().numLine,
                               tokens.back().numPos + 1, ""));
    }

    Poliz *translate() {
        // ~~~
        poliz = nowPoliz = new Poliz;
        // ~~~
        indexNowToken = 0;
        nowToken = &tokens[0];
        levels = {0};
        std::vector<std::string> reservedFunctions = {"print", "input", "int", "str", "bool", "len", "max", "min",
                                                      "double"};
        std::vector<std::string> reservedClasses = {"list", "dict", "set"};
        scopes.push_back({});
        bigScopes.push_back({});
        for (const std::string &cur : reservedFunctions) {
            scopes[0].functions[cur] = {};
            bigScopes[0].functions[cur] = {};
        }
        for (const std::string &cur : reservedClasses) {
            scopes[0].classes[cur] = {};
            bigScopes[0].classes[cur] = {};
        }
        try {
            readFileInput();
        } catch (Exception e) {
            std::string errorMessage;
            if (e.line != -1 && e.posInLine != -1) {
                errorMessage += std::to_string(e.line + 1) + ":" + std::to_string(e.posInLine + 1) + " ";
            }
            errorMessage += e.message;
            throw errorMessage;
        } catch (...) {
            std::cerr << "Semantic error";
        }
        return poliz;
    }

private:

    std::vector<Token> nameInTest;

    std::vector<int> levels;
    int indexNowToken{};
    Token *nowToken;

    void addElement(Element elem) {
        nowPoliz->operations.push_back(elem);
        nowPoliz->operations.back().numLine = nowToken->numLine;
        nowPoliz->operations.back().numPos = nowToken->numPos;
    }

    bool readBeginLine(int needMore = false) {
        if (nowToken->type != Token::BEGIN_LINE)
            return false;
        int level = std::stoi(nowToken->value);
        if (needMore) {
            if (level <= levels.back()) {
                throw Exception("invalid begin line",
                                nowToken->numLine,
                                nowToken->numPos);
            }
        }
        else {
            if (level > levels.back()) {
                throw Exception("invalid in begin of line",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            if (levels.back() > level) {
                return false;
            }
        }
        if (level != levels.back()) {
            levels.push_back(level);
        }
        getToken();
        return true;
    }

    bool readTest() {
        if (!readAndTest())
            return false;
        while (!isEnd()) {
            if (nowToken->value == "or") {
                getToken();
                if (!readAndTest()) {
                    throw Exception("invalid and expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                addElement({0, Element::OR});
                // ~~~
            }
            else {
                break;
            }
        }
        return true;
    }

    bool readCompOp(std::string *curOperator) {
        if (isOperator("<") || isOperator(">")
            || isOperator("==") || isOperator(">=")
            || isOperator("<=") || isOperator("!=")) {
            *curOperator = nowToken->value;
            getToken();
            return true;
        }
        return false;
    }

    bool readExpr() {
        if (!readXorExpr())
            return false;
        while (!isEnd()) {
            if (isOperator("|")) {
                getToken();
                if (!readXorExpr()) {
                    throw Exception("Invalid or expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                addElement({0, Element::OR_BIT});
                // ~~~

            }
            else {
                break;
            }
        }
        return true;
    }

    bool readXorExpr() {
        if (!readAndExpr())
            return false;
        while (!isEnd()) {
            if (isOperator("^")) {
                getToken();
                if (!readAndExpr()) {
                    throw Exception("invalid xor expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                addElement({0, Element::XOR});
                // ~~~
            }
            else {
                break;
            }
        }
        return true;
    }

    bool readAndExpr() {
        if (!readShiftExpr())
            return false;
        while (!isEnd()) {
            if (isOperator("&")) {
                getToken();
                if (!readShiftExpr()) {
                    throw Exception("invalid and expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                addElement({0, Element::AND_BIT});
                // ~~~
            }
            else {
                break;
            }
        }
        return true;
    }

    bool readShiftExpr() {
        if (!readArithExpr())
            return false;
        while (!isEnd()) {
            if (isOperator("<<")) {
                getToken();
                if (!readArithExpr()) {
                    throw Exception("invalid shift expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                addElement({0, Element::SHIFT_LEFT_BIN});
                // ~~~
                continue;
            }
            if (isOperator(">>")) {
                getToken();
                if (!readArithExpr()) {
                    throw Exception("invalid shift expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                addElement({0, Element::SHIFT_RIGHT_BIN});
                // ~~~
                continue;
            }
            else {
                break;
            }
        }
        return true;
    }

    bool readArithExpr() {
        if (!readTermExpr())
            return false;
        while (!isEnd()) {
            if (isOperator("+") || isOperator("-")) {
                // ~~~
                auto nowOperator = nowToken->value;
                // ~~~
                getToken();
                if (!readTermExpr()) {
                    throw Exception("invalid arith expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                if (nowOperator == "+") {
                    addElement({0, Element::PLUS}); // fixme по-хорошему
                }
                else {
                    addElement({0, Element::MINUS});
                }
                // ~~~
            }
            else {
                break;
            }
        }
        return true;
    }

    bool readTermExpr() {
        if (!readFactorExpr())
            return false;
        while (!isEnd()) {
            if (isOperator("*") || isOperator("/") ||
                isOperator("%") || isOperator("//")) {
                // ~~~
                auto nowOperator = nowToken->value;
                // ~~~
                getToken();
                if (!readFactorExpr()) {
                    throw Exception("Invalid arith expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                if (nowOperator == "*") {
                    addElement({0, Element::MUL});
                }
                else if (nowOperator == "/") {
                    addElement({0, Element::DIV});
                }
                else if (nowOperator == "//") {
                    addElement({0, Element::INTDIV});
                }
                else if (nowOperator == "%") {
                    addElement({0, Element::MOD});
                }
                // ~~~
            }
            else {
                break;
            }
        }
        return true;
    }

    bool readFactorExpr() {
        if (!isEnd()) {
            if (isOperator("-")
                || isOperator("+")
                || isOperator("~")) {
                // ~~~
                auto nowOperator = nowToken->value;
                // ~~~
                getToken();
                if (!readFactorExpr()) {
                    throw Exception("invalid factor expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                if (nowOperator == "-")
                    addElement({0, Element::UNAR_MINUS});
                if (nowOperator == "+")
                    addElement({0, Element::UNAR_PLUS});
                if (nowOperator == "~")
                    addElement({0, Element::UNAR_TILDA});
                // ~~~
                return true;
            }
            else {
                return readPowerExpr();
            }
        }
        else {
            return false;
        }
    }

    bool readPowerExpr() {
        if (!readNameExpr())
            return false;
        while (!isEnd()) {
            if (isOperator("**")) {
                getToken();
                if (!readNameExpr()) {
                    throw Exception("invalid power expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                addElement({0, Element::POW});
                // ~~~
            }
            else {
                break;
            }
        }
        return true;
    }

    bool isFuncDefined(std::string funcName) {
        auto it = bigScopes.back().functions.find({funcName});
        return it != bigScopes.back().functions.end();
    }

    bool isClassDefined(std::string funcName) {
        auto it = bigScopes.back().classes.find({funcName});
        return it != bigScopes.back().classes.end();
    }

    bool isFuncFullDefined(std::string funcName) {
        auto it = bigScopes.back().functions.find(funcName);
        if (it == bigScopes.back().functions.end()) {
            return false;
        }
        for (auto i = it->second.parentFunc.begin(); i != it->second.parentFunc.end();) {
            if (*i == funcName || isFuncFullDefined(*i)) {
                i = it->second.parentFunc.erase(i);
            }
            else {
                ++i;
            }
        }
        return it->second.parentFunc.empty();
    }

    bool isClassFullDefined(std::string className) {
        auto it = bigScopes.back().classes.find(className);
        if (it == bigScopes.back().classes.end()) {
            return false;
        }
        for (auto i = it->second.parentClasses.begin(); i != it->second.parentClasses.end();) {
            if (isClassFullDefined(*i)) {
                i = it->second.parentClasses.erase(i);
            }
            else {
                ++i;
            }
        }
        return it->second.parentClasses.empty();
    }

    bool readNameExpr() {
        if (isOperator("[")) {
            getToken();
            int cntElement = 0;
            if (!readTest()) {
                if (isOperator("]")) {
                    getToken();
                    addElement({0, Element::MAKE_LIST});
                    while (readTrailer());
                    return true;
                }
            }
            cntElement++;
            if (isOperator(",")) {
                while (isOperator(",")) {
                    cntElement++;
                    getToken();
                    if (!readTest()) {
                        throw Exception("expected testExpr",
                                        nowToken->numLine, nowToken->numPos);
                    }
                }
            }
            else if (isKeyword("for")) {
                getToken();
                if (nowToken->type != Token::NAME)
                    throw Exception("expected NAME",
                                    nowToken->numLine, nowToken->numPos);
                initVar(indexNowToken); // TODO list generator
//                checkDefinedTestNames();
                getToken();
                if (!isKeyword("in"))
                    throw Exception("expected in",
                                    nowToken->numLine, nowToken->numPos);
                getToken();
                if (!readTest())
                    throw Exception("expected testExpr",
                                    nowToken->numLine, nowToken->numPos);
            }
            if (!isOperator("]"))
                throw Exception("expected testExpr",
                                nowToken->numLine, nowToken->numPos);
            addElement({cntElement, Element::MAKE_LIST});
            getToken();
            while (readTrailer());
            return true;
        }
        if (isOperator("(")) {
            getToken();
            if (!readTest()) {
                throw Exception("invalid brackets statement expression",
                                nowToken->numLine, nowToken->numPos);
            }
            if (!isOperator(")")) {
                throw Exception("invalid brackets statement expression",
                                nowToken->numLine, nowToken->numPos);
            }
            getToken();
        }
        else if (nowToken->type == Token::NAME
                 || nowToken->type == Token::NUMBER
                 || nowToken->type == Token::STRING
                 || isKeyword("None")
                 || isKeyword("True")
                 || isKeyword("False")) {
            std::string name = nowToken->value;
            if (nowToken->type == Token::NAME) {
                if (isFuncDefined(name)) {
                    if (!isNextTokenOperator("(")) {
                        throw Exception("semantic error: " + nowToken->value + " defined as function",
                                        nowToken->numLine,
                                        nowToken->numPos);
                    }
                    else {
                        // ~~~
                        addElement({
                                           -1,
                                           Element::CALL_FUNC,
                                           0,
                                           0,
                                           0.0,
                                           nowToken->value
                                   });
                        // ~~~
                        /*
                        if (bigScopes.back().type == Scope::FUNC) {
//                            scopes.rbegin()->functions.find({bigScopes.back().value})->second.parentFunc.insert(
//                                    nowToken->value);
                            bigScopes.rbegin()->functions.find({bigScopes.back().value})->second.parentFunc.insert(
                                    nowToken->value);
                            next(bigScopes.rbegin())->functions.find(
                                    {bigScopes.back().value})->second.parentFunc.insert(
                                    nowToken->value);
                        }
                         else if (!isFuncFullDefined(name)) {
                            throw Exception("semantic error: " + nowToken->value + " not full defined",
                                            nowToken->numLine,
                                            nowToken->numPos);
                        }
                        */

                    }
                }
                else if (isClassDefined(name)) {
                    if (!isClassFullDefined(name)) {
                        throw Exception("semantic error: " + nowToken->value + " not full defined",
                                        nowToken->numLine,
                                        nowToken->numPos);
                    }

                    if (isNextTokenOperator("(")) {
                        if (bigScopes.back().type == Scope::CLASS) {
//                            scopes.rbegin()->functions.find({bigScopes.back().value})->second.parentFunc.insert(
//                                    nowToken->value);
                            bigScopes.rbegin()->classes.find({bigScopes.back().value})->second.parentClasses.insert(
                                    nowToken->value);
                            next(bigScopes.rbegin())->classes.find(
                                    {bigScopes.back().value})->second.parentClasses.insert(
                                    nowToken->value);
                            addElement({-1, Element::CREATE_CLASS, 0, 0, 0.0, nowToken->value});
                        }
                        else if (!isClassFullDefined(name)) {
                            throw Exception("semantic error: " + nowToken->value + " call undefined as function",
                                            nowToken->numLine,
                                            nowToken->numPos);
                        }
                        else {
                            addElement({-1, Element::CREATE_CLASS, 0, 0, 0.0, nowToken->value});
                        }
                    } else{
                        addElement({0, Element::GET_VALUE});
                        nowPoliz->operations[nowPoliz->operations.size() - 1].stringValue = nowToken->value;
                    }

                }
                    /*else if (!isDefines(name) && isNextTokenOperator("(")) {
                        if (bigScopes.back().type == Scope::FUNC) {
    //                            scopes.rbegin()->functions.find({bigScopes.back().value})->second.parentFunc.insert(
    //                                    nowToken->value);
                            bigScopes.rbegin()->functions.find({bigScopes.back().value})->second.parentFunc.insert(
                                    nowToken->value);
                            next(bigScopes.rbegin())->functions.find(
                                    {bigScopes.back().value})->second.parentFunc.insert(
                                    nowToken->value);
                        }
                        else
                            throw Exception("semantic error: " + nowToken->value + " not full defined",
                                            nowToken->numLine,
                                            nowToken->numPos);
                    }
    //                    else throw Exception("semantic error: " + nowToken->value + " defined as variable",
    //                                        nowToken->numLine,
    //                                        nowToken->numPos);*/
                else {
                    nameInTest.push_back(*nowToken);
                    addElement({0, Element::GET_VALUE});
                    nowPoliz->operations[nowPoliz->operations.size() - 1].stringValue = nowToken->value;
                }
            }
            else if (nowToken->type == Token::NUMBER) {
                // ~~~
                // TODO longInt
                if (std::find(nowToken->value.begin(), nowToken->value.end(), '.') == nowToken->value.end()) {
                    addElement({0,
                                Element::GET_VALUE_INT,
                                0,
                                std::stoi(nowToken->value)});
                }
                else {
                    addElement({0,
                                Element::GET_VALUE_DOUBLE,
                                0,
                                0,
                                std::stod(nowToken->value)});
                }
                // ~~~
            }
            else if (nowToken->type == Token::STRING) {
                // ~~~
                addElement({0,
                            Element::GET_VALUE_STR,
                            0,
                            0,
                            0.0,
                            nowToken->value
                           });
                // ~~~
            }
            else if (isKeyword("None")) {
                // ~~~
                addElement({0,
                            Element::GET_VALUE_NONE
                           });
                // ~~~
            }
            else if (isKeyword("True") || isKeyword("False")) {
                // ~~~
                addElement({0,
                            Element::GET_VALUE_BOOL,
                            0,
                            isKeyword("True")
                           });
                // ~~~
            }
            getToken();
        }
        else
            return false;
        while (readTrailer());
        return true;
    }

    bool readTrailer() {
        if (isOperator("(")) {
            getToken();
            auto backup = nowPoliz->operations.back();
            nowPoliz->operations.pop_back();
            int countArgs = readArglist();
            if (isOperator(")")) {
                getToken();
                backup.countParams = countArgs;
                addElement(backup);
                return true;
            }
            else
                throw Exception("invalid trailer",
                                nowToken->numLine, nowToken->numPos);
        }
        if (isOperator("[")) {
            getToken();
            if (!readTest()) {
                throw Exception("invalid trailer",
                                nowToken->numLine, nowToken->numPos);
            }
            addElement({-1, Element::INDEX_VALUE});
            if (isOperator("]")) {
                getToken();
                return true;
            }
            else
                throw Exception("invalid trailer",
                                nowToken->numLine, nowToken->numPos);
        }
        if (isOperator(".")) {
            getToken();
            if (nowToken->type != Token::NAME) {
                throw Exception("invalid trailer",
                                nowToken->numLine, nowToken->numPos);
            }
            else {
                auto name = nowToken->value;
                getToken();
                if (isOperator("(")) {
                    addElement({-1, Element::EVAL_METHOD, 0, 0, 0.0, name});
                }
                else {
                    addElement({-1, Element::GET_FIELD, 0, 0, 0.0, name});
                }
                return true;
            }
        }
        return false;
    }

    int readArglist() {
        if (!readTest())
            return 0;
        int countArgs = 1;
        while (!isEnd()) {
            if (isOperator(",")) {
                getToken();
                if (!readTest()) {
                    throw Exception("Invalid arglist",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                countArgs++;
            }
            else {
                break;
            }
        }
        return countArgs;
    }

    bool readComparison() {
        if (!readExpr()) {
            return false;
        }
        std::string nowOperator;
        if (!isEnd() && readCompOp(&nowOperator)) {
            if (!readExpr()) {
                throw Exception("invalid compare expression",
                                nowToken->numLine,
                                nowToken->numPos);
            }
        }
        if (nowOperator == "<") {
            addElement({0, Element::CMP_LESS});
        }
        if (nowOperator == ">") {
            addElement({0, Element::CMP_MORE});
        }
        if (nowOperator == "<=") {
            addElement({0, Element::CMP_LESS_EQUAL});
        }
        if (nowOperator == ">=") {
            addElement({0, Element::CMP_MORE_EQUAL});
        }
        if (nowOperator == "==") {
            addElement({0, Element::CMP_EQUAL});
        }
        if (nowOperator == "!=") {
            addElement({0, Element::CMP_NOT_EQUAL});
        }
        return true;
    }

    bool readNotTest() {
        if (isKeyword("not")) {
            getToken();
            if (!readNotTest()) {
                throw Exception("invalid not expression",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            // ~~~
            addElement({0, Element::NOT});
            // ~~~
        }
        else {
            if (!readComparison()) {
                return false;
            }
        }
        return true;
    }

    bool readAndTest() {
        if (!readNotTest())
            return false;
        while (!isEnd()) {
            if (isKeyword("and")) {
                getToken();
                if (!readNotTest()) {
                    throw Exception("invalid not expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                // ~~~
                addElement({0, Element::AND});
                // ~~~
            }
            else {
                break;
            }
        }
        return true;
    }

    bool readStmt() {
        return readCompoundStmt() || readSimpleStmt();
    }

    bool readDelStmt() {
        if (isKeyword("del")) {
            getToken();
            if (!readExpr()) {
                throw Exception("invalid delete stmt",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            addElement({0, Element::DEL});
            return true;
        }
        return false;
    }

    bool readPassStmt() {
        if (isKeyword("pass")) {
            getToken();
            return true;
        }
        return false;
    }

    bool readImportStmt() {
        if (isKeyword("import")) {
            getToken();
            if (nowToken->type != Token::NAME) {
                throw Exception("invalid import stmt",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            std::string filename = nowToken->value;
            std::string classname = filename;
            getToken();
            if (!isEnd()) {
                if (isKeyword("as")) {
                    getToken();
                    if (nowToken->type == Token::NAME) {
                        classname = nowToken->value;
                        initClass(*nowToken);
                        getToken();
                    }
                    else {
                        throw Exception("invalid import alias",
                                        nowToken->numLine,
                                        nowToken->numPos);
                    }
                }
            }
            std::string uclassname = "__" + classname;
            addElement({0, Element::IMPORT, 0,
                        static_cast<int>(filename.size()), 0, filename + uclassname});
            addElement({0, Element::GET_VALUE, 0,
                        0, 0, classname});
            addElement({0, Element::CREATE_CLASS, 0,
                        0, 0, uclassname});
            addElement({0, Element::COPY});
            return true;
        }
        return false;
    }

    bool readBreakStmt() {
        if (isKeyword("break")) {
            if (!bigScopes.back().countCycle) {
                throw Exception("semantic error: break outdoor cycle",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            breakPos.push_back(nowPoliz->operations.size());
            addElement({0, Element::JMP});
            getToken();
            return true;
        }
        return false;
    }

    void initVar(int pos) {
        std::string name = tokens[pos].value;
        if (bigScopes.back().functions.count({name}) == 1
            || bigScopes.back().classes.count({name}) == 1) {
            throw Exception("semantic error: redefinition \"" + name + "\"",
                            nowToken->numLine,
                            nowToken->numPos);
        }
        if (isNotNameDefines(name)) {
            scopes.back().variables.insert({name});
            bigScopes.back().variables.insert({name});
        }
    }

    bool isNotDefines(std::string name) {
        return bigScopes.back().variables.count({name}) != 1
               && bigScopes.back().functions.count(name) != 1
               && bigScopes.back().classes.count({name}) != 1;
    }

    bool isNotNameDefines(std::string name) {
        return bigScopes.back().variables.count({name}) != 1;
    }

    bool isNameDefines(std::string name) {
        return bigScopes.back().variables.count({name}) == 1;
    }

    bool checkDefinedTestNames() {
        for (auto now : nameInTest) {
            if (isNotNameDefines(now.value)) {
                throw Exception("semantic error: " + now.value + " not defined",
                                now.numLine,
                                now.numPos);
            }
        }
        nameInTest.clear();
        return true;
    }

    bool readExprStmt() {
        int pos = indexNowToken;
        if (!readTest())
            return false;
        bool isMove = false;
        if (pos + 1 == indexNowToken && tokens[pos].type == Token::NAME) {
            isMove = true;
            nameInTest.clear();
        }
        else {
            checkDefinedTestNames();
        }
        std::string operation;
        if (readAugassign(operation)) {
            if (isMove && isNotNameDefines(tokens[pos].value)) {
                throw Exception("semantic error: " + tokens[pos].value + " not defined",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            if (!readTest())
                throw Exception("invalid test",
                                nowToken->numLine,
                                nowToken->numPos);
            if (operation == "+=") {
                addElement({0, Element::PLUS_INPLACE});
            }
            else if (operation == "-=") {
                addElement({0, Element::MINUS_INPLACE});
            }
            else if (operation == "*=") {
                addElement({0, Element::MUL_INPLACE});
            }
            else if (operation == "**=") {
                addElement({0, Element::POW_INPLACE});
            }
            else if (operation == "/=") {
                addElement({0, Element::DIV_INPLACE});
            }
            else if (operation == "//=") {
                addElement({0, Element::INTDIV_INPLACE});
            }
            else if (operation == "^=") {
                addElement({0, Element::XOR_INPLACE});
            }
            else if (operation == "&=") {
                addElement({0, Element::AND_BIT_INPLACE});
            }
            else if (operation == "|=") {
                addElement({0, Element::OR_BIT_INPLACE});
            }
            else if (operation == ">>=") {
                addElement({0, Element::RIGHT_SHIFT_INPLACE});
            }
            else if (operation == "<<=") {
                addElement({0, Element::LEFT_SHIFT_INPLACE});
            }
            else if (operation == "%=") {
                addElement({0, Element::MOD_INPLACE});
            }
            // clear stack is a trash
        }
        else {
            while (isOperator("=")) {
                if (isMove) {
                    initVar(pos);
                }
                getToken();
                pos = indexNowToken;
                if (!readTest()) {
                    throw Exception("invalid test expr",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                addElement({0, Element::COPY});
                addElement({0, Element::CLEAR_STACK});
                isMove = false;
                if (isNextTokenOperator("=") && pos + 1 == indexNowToken && tokens[pos].type == Token::NAME) {
                    isMove = true;
                    nameInTest.clear(); // FIXME a = a
                }
                else {
                    checkDefinedTestNames();
                }
            }
        }
        return true;
    }

    bool readAugassign(std::string &s) {
        if (isOperator("+=") || isOperator("-=")
            || isOperator("*=") || isOperator("/=")
            || isOperator("%=") || isOperator("**=")
            || isOperator("&=") || isOperator("|=")
            || isOperator("^=") || isOperator("<<=")
            || isOperator(">>=")) {
            s = nowToken->value;
            getToken();
            return true;
        }
        return false;
    }

    bool readContinueStmt() {
        if (isKeyword("continue")) {
            if (!bigScopes.back().countCycle) {
                throw Exception("semantic error: continue outdoor cycle",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            getToken();
            continuePos.push_back(nowPoliz->operations.size());
            addElement({0, Element::JMP});
            return true;
        }
        return false;
    }

    bool readReturnStmt() {
        if (isKeyword("return")) {
            if (!bigScopes.back().countFunc) {
                throw Exception("semantic error: return outdoor function",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            getToken();
            readTest();
            checkDefinedTestNames();
            // ~~~
            addElement({0, Element::RETURN_VALUE});
            // ~~~
            return true;
        }
        return false;
    }

    bool readFlowStmt() {
        return (readBreakStmt() || readContinueStmt() || readReturnStmt());
    }

    bool readSimpleStmt() {
        return readDelStmt() || readPassStmt()
               || readImportStmt() || readFlowStmt()
               || readExprStmt();
    }

    bool readCompoundStmt() {
        return readIfStmt() || readWhileStmt()
               || readForStmt() || readFuncdef()
               || readClassdef();
    }

    void popBigScope() {
        for (auto i = scopes.back().functions.begin(); i != scopes.back().functions.end(); ++i) {
            if (i->first == bigScopes.back().value)
                continue;

            if (!i->second.parentFunc.empty()) {
                throw Exception("semantic error: " + bigScopes.back().value + " not full defined",
                                nowToken->numLine,
                                nowToken->numPos);
            }
        }

        for (auto i = scopes.back().classes.begin(); i != scopes.back().classes.end(); ++i) {
            if (i->first == bigScopes.back().value)
                continue;

            if (!i->second.parentClasses.empty()) {
                throw Exception("semantic error: " + bigScopes.back().value + " not full defined",
                                nowToken->numLine,
                                nowToken->numPos);
            }
        }
        bigScopes.pop_back();
    }

    void initFunc(Token token) {
        if (isNotDefines(token.value)) {
            for (auto &now : bigScopes.back().functions) {
                now.second.parentFunc.erase(token.value);
            }
            scopes.back().functions[token.value] = {};
            bigScopes.back().functions[token.value] = {};
        }
        else {
            throw Exception("semantic error: redefinition " + token.value,
                            nowToken->numLine,
                            nowToken->numPos);
        }
    }

    void initClass(Token token) {
        if (isNotDefines(token.value)) {
            scopes.back().classes[token.value] = {};
            bigScopes.back().classes[token.value] = {};
        }
        else {
            throw Exception("semantic error: redefinition " + token.value,
                            nowToken->numLine,
                            nowToken->numPos);
        }
    }

    bool readFuncdef() {
        if (nowToken->value != "def")
            return false;
        getToken();
        auto mainPoliz = nowPoliz;
        if (isDefines(nowToken->value))
            throw Exception("semantic error: redefinition " + nowToken->value,
                            nowToken->numLine,
                            nowToken->numPos);
        if (nowToken->type != Token::NAME)
            throw Exception("expected NAME after def",
                            nowToken->numLine,
                            nowToken->numPos);
        int index = nowPoliz->operations.size();
        addElement({0, Element::DEF_FUNC, 0,
                    0, 0, nowToken->value});
        auto funcPoliz = new Poliz;
        mainPoliz->funcs[nowToken->value] = funcPoliz;
        nowPoliz = funcPoliz;
        nowPoliz->funcs[nowToken->value] = nowPoliz;

        initFunc(*nowToken);
        Scope last = bigScopes.back();
        bigScopes.push_back({Scope::FUNC, nowToken->value, 0, 1});
        bigScopes.back().classes = last.classes;
        bigScopes.back().functions = last.functions;
        bigScopes.back().variables = last.variables;
        scopes.push_back({Scope::FUNC, nowToken->value});
        getToken();
        int countParams = 0;
        if (!readParameters(countParams))
            throw Exception("invalid function parameters",
                            nowToken->numLine,
                            nowToken->numPos);
        if (!isBeginBlock(":"))
            throw Exception("expected : after function parameters",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!readSuite()) {
            throw Exception("invalid function suite",
                            nowToken->numLine,
                            nowToken->numPos);
        }
        popScope();
        popBigScope();
        nowPoliz = mainPoliz;
        nowPoliz->operations[index].countParams = countParams;
        return true;
    }

    bool readParameters(int &countParams) {
        if (!isOperator("("))
            return false;
        getToken();
        if (isOperator(")")) {
            getToken();
            return true;
        }
        ++countParams;
        if (!readFuncdefarglist(countParams))
            throw Exception("invalid arglist",
                            nowToken->numLine,
                            nowToken->numPos);
        if (!isOperator(")"))
            throw Exception("expected )",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        return true;
    }

    bool readFuncdefarglist(int &countParams) {
        if (nowToken->type != Token::NAME)
            throw Exception("expected name",
                            nowToken->numLine,
                            nowToken->numPos);
        if (isDefines(nowToken->value)) {
            throw Exception("semantic error: " + nowToken->value + " already used",
                            nowToken->numLine,
                            nowToken->numPos);
        }
        std::vector<Element> params;
        scopes.back().variables.insert({nowToken->value});
        bigScopes.back().variables.insert({nowToken->value});
        initVar(indexNowToken);
        params.push_back({0, Element::INIT_PARAM, 0, 0, 0.0, nowToken->value});
        getToken();
        while (isOperator(",")) {
            ++countParams;
            getToken();
            if (nowToken->type != Token::NAME)
                throw Exception("expected name",
                                nowToken->numLine,
                                nowToken->numPos);
            if (isNameDefines(nowToken->value)) {
                throw Exception("semantic error: " + nowToken->value + " already used",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            initVar(indexNowToken);
            params.push_back({0, Element::INIT_PARAM, 0, 0, 0.0, nowToken->value});
            getToken();
        }
        for (int i = params.size() - 1; i >= 0; i--) {
            addElement(params[i]);
        }
        return true;
    }

    void popScope() {
        for (auto now : scopes.back().variables) {
            bigScopes.back().variables.erase({now});
        }
        scopes.pop_back();
    }

    bool readWhileStmt() {
        if (nowToken->value != "while")
            return false;
        getToken();
        // ~~~
        addElement({0, Element::BEGIN_SCOPE});
        int statementBeginPos = nowPoliz->operations.size();
        // ~~~
        scopes.push_back({Scope::SIMPLE, ""});
        bigScopes.back().countCycle++;
        if (!readTest())
            throw Exception("invalid while condition",
                            nowToken->numLine,
                            nowToken->numPos);
        if (!isBeginBlock(":"))
            throw Exception("invalid while condition",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        // ~~~
        int suiteBeginPos = nowPoliz->operations.size();
        addElement({0, Element::NEGATIVE_JMP, 0});
        int stmtJmpPos = nowPoliz->operations.size() - 1;
        bool withElse = false;
        // ~~~
        readSuite();
        // ~~~
        addElement({0, Element::JMP, statementBeginPos});
        int elseSuiteBegin = 0;
        // ~~~
        addElement({0, Element::END_SCOPE});
        if (isEqualLevel() && isNextTokenKey("else")) {
            withElse = true;
            readBeginLine();
            getToken();
            addElement({0, Element::BEGIN_SCOPE});
            scopes.push_back({});
            if (!isBeginBlock(":"))
                throw Exception("expected : after else",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
            elseSuiteBegin = nowPoliz->operations.size();
            readSuite();
            popScope();
            addElement({0, Element::END_SCOPE});
        }
        // ~~~
        int endPos = nowPoliz->operations.size();
        if (withElse)
            endPos = elseSuiteBegin;
        nowPoliz->operations[stmtJmpPos].posJump = endPos;
        while (!breakPos.empty() && breakPos.back() >= stmtJmpPos) {
            nowPoliz->operations[breakPos.back()].posJump = endPos;
            breakPos.pop_back();
        }
        while (!continuePos.empty() && continuePos.back() >= stmtJmpPos) {
            nowPoliz->operations[continuePos.back()].posJump = statementBeginPos;
            continuePos.pop_back();
        }
        // ~~~
        return true;
    }

    bool readForStmt() {
        if (nowToken->value != "for")
            return false;
        getToken();
        // ~~~
        addElement({0, Element::BEGIN_SCOPE});
        int badJmpPos = 0;
        // ~~~
        scopes.push_back({Scope::SIMPLE, ""});
        bigScopes.back().countCycle++;
        if (nowToken->NAME != Token::NAME)
            throw Exception("expected name after for",
                            nowToken->numLine,
                            nowToken->numPos);
        std::string name = nowToken->value;
        std::string nameLen = "__len" + std::to_string(bigScopes.back().countCycle);
        std::string nameIter = "__iter" + std::to_string(bigScopes.back().countCycle);
        std::string nameArray = "__array" + std::to_string(bigScopes.back().countCycle);
        initVar(indexNowToken);
        getToken();
        if (!isKeyword("in"))
            throw Exception("expected in after for in",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        // ~~~
        addElement({0, Element::GET_VALUE, 0, 0, 0, nameArray});
        // ~~~
        if (!readTest() && checkDefinedTestNames())
            throw Exception("invalid for condition",
                            nowToken->numLine,
                            nowToken->numPos);
        // ~~~
        addElement({0, Element::COPY});

        addElement({0, Element::GET_VALUE, 0, 0, 0, nameLen});
        addElement({0, Element::GET_VALUE, 0, 0, 0, nameArray});
        addElement({0, Element::CALL_FUNC, 0, 0, 0, "len"});
        addElement({0, Element::COPY});

        addElement({0, Element::GET_VALUE, 0, 0, 0, nameIter});
        addElement({0, Element::GET_VALUE_INT});
        addElement({0, Element::COPY});
        // ~~~
        if (!isBeginBlock(":"))
            throw Exception("expected : after for statement",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        // ~~~
        int stmtBeginPos = nowPoliz->operations.size();
        addElement({0, Element::GET_VALUE, 0, 0, 0, nameIter});
        addElement({0, Element::GET_VALUE, 0, 0, 0, nameLen});
        addElement({0, Element::CMP_LESS});
        int jmpToEnd = nowPoliz->operations.size();
        addElement({0, Element::NEGATIVE_JMP});
        // ~~~

        // ~~~
        addElement({0, Element::GET_VALUE, 0, 0, 0, name});
        addElement({0, Element::GET_VALUE, 0, 0, 0, nameArray});
        addElement({0, Element::GET_VALUE, 0, 0, 0, nameIter});
        addElement({0, Element::INDEX_VALUE});
        addElement({0, Element::COPY});
        // ~~~
        if (!readSuite())
            throw Exception("invalid for suite",
                            nowToken->numLine,
                            nowToken->numPos);

        popScope();
        // ~~~
        addElement({0, Element::GET_VALUE, 0, 0, 0, nameIter});
        addElement({0, Element::GET_VALUE_INT, 0, 1});
        addElement({0, Element::PLUS_INPLACE});
        addElement({0, Element::JMP, stmtBeginPos});
        int withElse = false;
        addElement({0, Element::END_SCOPE});
        // ~~~
        if (isEqualLevel() && isNextTokenKey("else")) {
            readBeginLine();
            getToken();
            // ~~~
            withElse = true;
            addElement({0, Element::BEGIN_SCOPE});
            // ~~~
            scopes.push_back({});
            if (!isBeginBlock(":"))
                throw Exception("expected : after else",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
            badJmpPos = nowPoliz->operations.size();
            readSuite();
            popScope();
            // ~~~
            addElement({0, Element::END_SCOPE});
            // ~~~
        }
        if (!withElse)
            badJmpPos = nowPoliz->operations.size();
        nowPoliz->operations[jmpToEnd].posJump = badJmpPos;
        bigScopes.back().countCycle--;
        return true;
    }

    bool readSuite() {
        if (!readBeginLine(true)) {
            throw Exception("expected suite",
                            nowToken->numLine,
                            nowToken->numPos);
        }
        if (!readStmt()) {
            throw Exception("expected suite",
                            nowToken->numLine,
                            nowToken->numPos);
        }
        while (readBeginLine()) {
            if (nowToken->type != Token::BEGIN_LINE && !readStmt()) {
                throw Exception("invalid suite kek",
                                nowToken->numLine,
                                nowToken->numPos);
            }
        }
        popLevel();
        return true;
    }

    bool readIfStmt() {
        if (!isKeyword("if"))
            return false;
        getToken();
        scopes.push_back({Scope::SIMPLE, ""});
        if (!readTest() && checkDefinedTestNames())
            throw Exception("invalid if condition",
                            nowToken->numLine,
                            nowToken->numPos);
        if (!isBeginBlock(":")) {
            throw Exception("invalid if condition",
                            nowToken->numLine,
                            nowToken->numPos);
        }
        getToken();
        // ~~~
        int stmtPos = nowPoliz->operations.size();
        addElement({0, Element::NEGATIVE_JMP});
        int endPos = -1;
        std::vector<int> elifEndPos;
        // ~~~
        addElement({0, Element::BEGIN_SCOPE});
        readSuite();
        addElement({0, Element::END_SCOPE});
        // ~~~
        elifEndPos.push_back(nowPoliz->operations.size());
        addElement({0, Element::JMP});
        // ~~~
        popScope();
        while (isEqualLevel() && isNextTokenKey("elif")) {
            readBeginLine();
            getToken();
            // ~~~
            nowPoliz->operations[stmtPos].posJump = nowPoliz->operations.size();
            // ~~~
            scopes.push_back({});
            if (!readTest() && checkDefinedTestNames()) {
                throw Exception("invalid elif condition",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            // ~~~
            stmtPos = nowPoliz->operations.size();
            addElement({0, Element::NEGATIVE_JMP});
            // ~~~
            if (!isBeginBlock(":")) {
                throw Exception("invalid elif condition",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            getToken();
            addElement({0, Element::BEGIN_SCOPE});
            readSuite();
            addElement({0, Element::END_SCOPE});
            // ~~~
            elifEndPos.push_back(nowPoliz->operations.size());
            addElement({0, Element::JMP});
            // ~~~
            popScope();
        }
        bool withElse = false;
        if (isEqualLevel() && isNextTokenKey("else")) {
            withElse = true;
            readBeginLine();
            getToken();
            scopes.push_back({});
            if (!isBeginBlock(":"))
                throw Exception("expected : after else statements",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
            // ~~~
            nowPoliz->operations[stmtPos].posJump = nowPoliz->operations.size();
            // ~~~
            addElement({0, Element::BEGIN_SCOPE});
            readSuite();
            addElement({0, Element::END_SCOPE});
            popScope();
        }
        // ~~~
        endPos = nowPoliz->operations.size();
        for (int i:elifEndPos)
            nowPoliz->operations[i].posJump = endPos;
        if (!withElse)
            nowPoliz->operations[stmtPos].posJump = endPos;
        // ~~~
        return true;
    }

    bool isDefines(std::string name) {
        return bigScopes.back().variables.count({name})
               || bigScopes.back().functions.count(name)
               || bigScopes.back().classes.count({name});
    }

    bool readClassdef() {
        if (!isKeyword("class"))
            return false;
        getToken();
        auto mainPoliz = nowPoliz;
        if (isDefines(nowToken->value))
            throw Exception("semantic error: redefinition " + nowToken->value,
                            nowToken->numLine,
                            nowToken->numPos);
        if (nowToken->type != Token::NAME)
            throw Exception("expected class name",
                            nowToken->numLine,
                            nowToken->numPos);
        int index = nowPoliz->operations.size();
        addElement({0, Element::DEF_CLASS, 0,
                    0, 0, nowToken->value});
        auto classPoliz = new Poliz;
        mainPoliz->classes[nowToken->value] = classPoliz;
        nowPoliz = classPoliz;
        nowPoliz->classes[nowToken->value] = nowPoliz;

        initClass(*nowToken);
        Scope last = bigScopes.back();
        bigScopes.push_back({Scope::CLASS, nowToken->value, 0, 0});
        bigScopes.back().classes = last.classes;
        bigScopes.back().functions = last.functions;
        scopes.push_back({Scope::CLASS, nowToken->value});
        getToken();
        int countParams = 0;
        if (!isBeginBlock(":"))
            throw Exception("expected : invalid classdef",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!readSuite()) {
            throw Exception("Invalid class suite",
                            nowToken->numLine,
                            nowToken->numPos);

        }
        popScope();
        popBigScope();
        nowPoliz = mainPoliz;
        nowPoliz->operations[index].countParams = 0;
        return true;
    }

    void readFileInput() {
        while (readBeginLine()) {
            readStmt();
        }
        if (levels.size() > 1) {
            throw Exception("invalid EOF");
        }
        if (!isEnd())
            throw Exception("unknown syntax",
                            nowToken->numLine,
                            nowToken->numPos);
    }

    bool isEnd() {
        return nowToken->type == Token::ENDMARKER;
    }

    void getToken() {
        if (indexNowToken + 1 < tokens.size()) {
            indexNowToken++;
            nowToken = &tokens[indexNowToken];
        }
        else {
            throw Exception("invalid EOF",
                            nowToken->numLine,
                            nowToken->numPos);
        }
    }

    bool isNextTokenKey(std::string name) {
        return (indexNowToken + 1 < tokens.size()
                && tokens[indexNowToken + 1].type == Token::KEYWORD
                && tokens[indexNowToken + 1].value == name);
    }

    bool isNextTokenOperator(std::string name) {
        return (indexNowToken + 1 < tokens.size()
                && tokens[indexNowToken + 1].type == Token::OPERATOR
                && tokens[indexNowToken + 1].value == name);
    }

    bool isNextTokenBeginLine(std::string name) {
        return (indexNowToken + 1 < tokens.size()
                && tokens[indexNowToken + 1].type == Token::BEGIN_LINE);
    }

    bool isEqualLevel() {
        if (nowToken->type == Token::ENDMARKER)
            return levels.back() == 0;
        if (nowToken->type != Token::BEGIN_LINE)
            throw Exception("invalid EOL",
                            nowToken->numLine,
                            nowToken->numPos);
        return std::stoi(nowToken->value) == levels.back();
    }

    bool isKeyword(const std::string &name) {
        return nowToken->type == Token::KEYWORD && nowToken->value == name;
    }

    bool isOperator(const std::string &name) {
        return nowToken->type == Token::OPERATOR && nowToken->value == name;
    }

    bool isBeginBlock(const std::string &name) {
        return nowToken->type == Token::BEGIN_BLOCK && nowToken->value == name;
    }

    void popLevel() {
        levels.pop_back();
    }
};

#endif //WOW_TRANSLATOR_H
