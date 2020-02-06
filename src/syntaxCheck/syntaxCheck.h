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
// TODO обернуть всё в try чтобы ловить случайные вылеты на границу tokens
// или хз
private:
    std::vector<int> levels;
    int nowToken{};

    bool readBeginLine(int needMore = false) {
        if (isEnd() || tokens[nowToken].type != Token::BEGIN_LINE)
            return false;
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
        getToken();
        return true;
    }

    bool readTest() {
        if (!readAndTest())
            return false;
        while (!isEnd()) {
            if (tokens[nowToken].value == "or") {
                getToken();
                if (!readAndTest()) {
                    throw Exception("Invalid and expression",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readCompOp() {
        if (tokens[nowToken].value == "<"
            || tokens[nowToken].value == ">"
            || tokens[nowToken].value == "=="
            || tokens[nowToken].value == ">="
            || tokens[nowToken].value == "<="
            || tokens[nowToken].value == "!=") {
            getToken();
            return true;
        }
        return false;
    }


    bool readExpr() {
        if (!readXorExpr())
            return false;
        while (!isEnd()) {
            if (tokens[nowToken].value == "|") {
                getToken();
                if (!readXorExpr()) {
                    throw Exception("Invalid or expression",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readXorExpr() {
        if (!readAndExpr())
            return false;
        while (!isEnd()) {
            if (tokens[nowToken].value == "^") {
                getToken();
                if (!readAndExpr()) {
                    throw Exception("Invalid xor expression",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readAndExpr() {
        if (!readShiftExpr())
            return false;
        while (!isEnd()) {
            if (tokens[nowToken].value == "&") {
                getToken();
                if (!readShiftExpr()) {
                    throw Exception("Invalid and expression",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readShiftExpr() {
        if (!readArithExpr())
            return false;
        while (!isEnd()) {
            if (tokens[nowToken].value == "<<" || tokens[nowToken].value == ">>") {
                getToken();
                if (!readArithExpr()) {
                    throw Exception("Invalid shift expression",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readArithExpr() {
        if (!readTermExpr())
            return false;
        while (!isEnd()) {
            if (tokens[nowToken].value == "+" || tokens[nowToken].value == "-") {
                getToken();
                if (!readTermExpr()) {
                    throw Exception("Invalid arith expression",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readTermExpr() {
        if (!readFactorExpr())
            return false;
        while (!isEnd()) {
            if (tokens[nowToken].value == "*" || tokens[nowToken].value == "/" ||
                tokens[nowToken].value == "%" || tokens[nowToken].value == "//") {
                getToken();
                if (!readFactorExpr()) {
                    throw Exception("Invalid arith expression",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readFactorExpr() {
        while (!isEnd()) {
            if (tokens[nowToken].value == "-" || tokens[nowToken].value == "+" ||
                tokens[nowToken].value == "~") {
                getToken();
                if (!(readFactorExpr() || readPowerExpr())) {
                    throw Exception("Invalid factor expression",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readPowerExpr() {
        if (!readNameExpr())
            return false;
        while (!isEnd()) {
            if (tokens[nowToken].value == "**") {
                getToken();
                if (!readNameExpr()) {
                    throw Exception("Invalid power expression",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readNameExpr() {
        if (tokens[nowToken].value == "(") {
            getToken();
            if (!readTest()) {
                throw Exception("Invalid brackets statement expression",
                                tokens[nowToken].numLine, tokens[nowToken].numPos);
            }
            if (tokens[nowToken].value != ")") {
                throw Exception("Invalid brackets statement expression",
                                tokens[nowToken].numLine, tokens[nowToken].numPos);
            }
            getToken();
        } else if (tokens[nowToken].type == Token::NAME
                   || tokens[nowToken].type == Token::NUMBER
                   || tokens[nowToken].type == Token::STRING
                   || tokens[nowToken].value == "None"
                   || tokens[nowToken].value == "True"
                   || tokens[nowToken].value == "False")
                getToken();
        else
            throw Exception("Invalid name expression",
                                  tokens[nowToken].numLine, tokens[nowToken].numPos);
        while(readTrailer());
    }

    bool readTrailer(){
        if (tokens[nowToken].value == "("){
            getToken();
            readArglist();
            if (tokens[nowToken].value == ")") {
                getToken();
                return true;
            }
            else
                throw Exception("Invalid trailer",
                                tokens[nowToken].numLine, tokens[nowToken].numPos);
        }
        if (tokens[nowToken].value == "["){
            getToken();
            readTest();
            if (tokens[nowToken].value == "]") {
                getToken();
                return true;
            }
            else
                throw Exception("Invalid trailer",
                                tokens[nowToken].numLine, tokens[nowToken].numPos);
        }
        if (tokens[nowToken].value == "."){
            getToken();
            if (tokens[nowToken].type != Token::NAME){
                throw Exception("Invalid trailer",
                                tokens[nowToken].numLine, tokens[nowToken].numPos);
            } else{
                getToken();
                return true;
            }
        }
        return false;
    }

    bool readArglist() {
        if (!readTest())
            return false;
        while (!isEnd()) {
            if (tokens[nowToken].value == ",") {
                getToken();
                if (!readTest()) {
                    throw Exception("Invalid arglist",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }



    bool readComparison() {
        if (!readExpr()) {
            return false;
        }
        if (!isEnd() && readCompOp()) {
            if (!readExpr()) {
                throw Exception("invalid compare expression",
                                tokens[nowToken].numLine,
                                tokens[nowToken].numPos);
            }
        }
    }

    bool readNotTest() {
        if (tokens[nowToken].value == "not") {
            getToken();
            if (!readNotTest()) {
                throw Exception("invalid not expression",
                                tokens[nowToken].numLine, tokens[nowToken].numPos);
            }
        } else {
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
            if (tokens[nowToken].value == "and") {
                getToken();
                if (!readNotTest()) {
                    throw Exception("Invalid not expression",
                                    tokens[nowToken].numLine,
                                    tokens[nowToken].numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readStmt() {
        if (readCompoundStmt())
            return true;
        return readSimpleStmt();
    }


    bool readDelStmt() {
        if (tokens[nowToken].value == "del") {
            getToken();
            if (!readExpr()) {
                throw Exception("invalid delete stmt",
                                tokens[nowToken].numLine,
                                tokens[nowToken].numPos); // FIXME stmt
            }
        }
    }

    bool readPassStmt() {
        if (tokens[nowToken].value == "pass") {
            getToken();
        }
    }

    bool nowImportStmt() {
        if (tokens[nowToken].value == "import") {
            getToken();
            if (tokens[nowToken].type != Token::NAME) {
                throw Exception("invalid import stmt",
                                tokens[nowToken].numLine,
                                tokens[nowToken].numPos);
            }
            if (!isEnd()) {
                if (tokens[nowToken].value == "import") {
                    getToken();
                    if (tokens[nowToken].type == Token::NAME) {
                        getToken();
                    } else {
                        throw Exception("invalid import alias",
                                        tokens[nowToken].numLine,
                                        tokens[nowToken].numPos);
                    }
                }
            }
        }
    }

    bool readBreakStmt() {
        if (tokens[nowToken].value == "break") {
            getToken();
            return true;
        }
        return false;
    }

    bool readContinueStmt() {
        if (tokens[nowToken].value == "continue") {
            getToken();
            return true;
        }
        return false;
    }

    bool readReturnStmt() {
        if (tokens[nowToken].value == "return") {
            getToken();
            readTest();
            return true;
        }
        return false;
    }

    bool readFlowStmt() {
        return (readBreakStmt() || readContinueStmt() || readReturnStmt());
    }

    bool readSimpleStmt() {
        return readDelStmt() || readPassStmt()
               || nowImportStmt() || readFlowStmt()
               || readExpr();
    }

    bool readCompoundStmt() {
        return readIfStmt() || readWhileStmt()
               || readForStmt() || readFuncdefStmt()
               || readClassdefStmt();
    }

    bool readSuite() {
        if (!readBeginLine(true)) {
            return false;
        }
        while (readBeginLine()) {
            while (readBeginLine()) {}

            if (!readSimpleStmt())
                return false;
        }
        return readStmt();
    }

    bool readIfStmt() {
        if (tokens[nowToken].value != "if")
            return false;
        getToken();
        if (!readTest())
            throw Exception("invalid if condition", tokens[nowToken].numLine,
                            tokens[nowToken].numPos);
        if (tokens[nowToken].value != ":") {
            throw Exception("expected : after if statements", tokens[nowToken].numLine,
                            tokens[nowToken].numPos);
        }
        getToken();
        readBeginLine(true);
        readSuite();
        while (tokens[nowToken].value == "elif") {
            getToken();
            if (!readTest())
                throw Exception("invalid if condition", tokens[nowToken].numLine,
                                tokens[nowToken].numPos);
            if (tokens[nowToken].value != ":")
                throw Exception("expected : after elif statements", tokens[nowToken].numLine,
                                tokens[nowToken].numPos);
            getToken();
            readBeginLine(true);
        }
        if (tokens[nowToken].value == "else") {
            getToken();
            if (tokens[nowToken].value != ":")
                throw Exception("expected : after else statements", tokens[nowToken].numLine,
                                tokens[nowToken].numPos);
            readBeginLine(true);
            readSuite();
        }
    }

    bool readClassdef() {
        if (tokens[nowToken].value != "class")
            return false;
        getToken();
        if (tokens[nowToken].type != Token::NAME)
            throw Exception("expected class name", tokens[nowToken].numLine,
                            tokens[nowToken].numPos);
        getToken();
        if (tokens[nowToken].value != ":")
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

    bool isEnd() {
        return nowToken >= tokens.size();
    }

    void getToken() {
        if (nowToken + 1 < tokens.size())
            nowToken++;
        throw Exception("invalid EOL");
    }
};


#endif //WOW_SYNTAXCHECK_H
