#ifndef WOW_SYNTAXCHECK_H
#define WOW_SYNTAXCHECK_H

#include <vector>
#include "../tokenizer/tokenizer.h"

class SyntaxCheck {
    std::vector<Token> tokens;
public:
    explicit SyntaxCheck(std::vector<Token> &v) : tokens(v) {}

    void check() {
        nowToken = 0;
        levels = {0};
        file_input_parse();
    }
// Tip в классе Exception есть поля строка и позиция
private:
    std::vector<int> levels;
    int nowToken{};

    void readBeginLine(int needMore = false) {
        if (tokens[nowToken].type == Token::BEGIN_LINE) {
            int level = std::stoi(tokens[nowToken].value);
            if (needMore) {
                if (level <= levels.back()) {
                    throw Exception("invalid begin line");
                }
            } else {
                if (level > levels.back()) {
                    throw Exception("Invalid in begin of line", tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
                while (levels.back() <= level) {
                    levels.pop_back();
                }
            }
            levels.push_back(level);
        } else {
            throw Exception("invalid begin line");
        }
    }

    bool nextTokenIsNewline() {
        return nowToken + 1 < tokens.size() && tokens[nowToken + 1].type == Token::BEGIN_LINE;
    }

    bool readTestStmt(){
        readOrTest();
    }

    bool readOrTest(){
        readAndTest();
        if(){
            
        }
    }
 
    bool readStmt() {
        if (readCompoundStmt())
            return true;
        return readSimpleStmt();
    }

    bool readSimpleStmt() {
        if (readDelStmt())
            return true;
        if (readPassStmt())
            return true;
        if (nowImportStmt())
            return true;
        if (readFlowStmt())
            return true;
        return readExprStmt();
    }

    bool readCompoundStmt() {
        if (readIfStmt())
            return true;
        if (readWhileStmt())
            return true;
        if (readForStmt())
            return true;
        if (readFuncdefStmt())
            return true;
        return (readClassdefStmt());
    }

    bool readSuiteStmt() {
        if(readSimpleStmt())
            return true;
        return readStmt();
    }

    bool readIfStmt() {
        if (tokens[nowToken].value != "if")
            return false;
        getToken();
        if (!readTestStmt())
            throw Exception("invalid if condition", tokens[nowToken].numLine,
                            tokens[nowToken].numPos);
        if (tokens[nowToken].value != ":") {
            throw Exception("expected : after if statements", tokens[nowToken].numLine,
                            tokens[nowToken].numPos);
        }
        getToken();
        readBeginLine(true);
        readSuiteStmt();
        while (tokens[nowToken].value == "elif") {
            getToken();
            if (!readTestStmt())
                throw Exception("invalid if condition", tokens[nowToken].numLine,
                                tokens[nowToken].numPos);
            if (tokens[nowToken].value != ":")
                throw Exception("expected : after elif statements", tokens[nowToken].numLine,
                                      tokens[nowToken].numPos);
            getToken();
            readBeginLine(true);
        }
        if (tokens[nowToken].value == "else"){
            getToken();
            if (tokens[nowToken].value != ":")
                throw Exception("expected : after else statements", tokens[nowToken].numLine,
                                tokens[nowToken].numPos);
            readBeginLine(true);
            readSuiteStmt();
        }
    }

    void file_input_parse() {
        while (nowToken < tokens.size()) {
            readBeginLine();
            readStmt();
        }
        if (levels.size() > 1) {
            throw Exception("invalid EOF");
        }
    }

    Token getToken() {
        if (nowToken + 1 < tokens.size())
            nowToken++;
        throw Exception("invalid EOL")
    }
};


#endif //WOW_SYNTAXCHECK_H
