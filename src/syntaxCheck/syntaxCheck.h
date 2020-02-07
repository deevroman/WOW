#ifndef WOW_SYNTAXCHECK_H
#define WOW_SYNTAXCHECK_H

#include <vector>
#include "../tokenizer/tokenizer.h"

class SyntaxCheck {
    std::vector<Token> tokens;
public:
    explicit SyntaxCheck(std::vector<Token> &v) : tokens(v) {
        v.push_back(Token(Token::ENDMARKER, -1, -1, ""));
    }

    void check() {
        indexNowToken = 0;
        nowToken = &tokens[0];
        levels = {0};
        file_input_parse();
    }
// Tip в классе Exception есть поля строка и позиция
// TODO обернуть всё в try чтобы ловить случайные вылеты на границу tokens
// или хз
private:
    std::vector<int> levels;
    int indexNowToken{};
    Token *nowToken;

    bool readBeginLine(int needMore = false) {
        if (nowToken->type != Token::BEGIN_LINE)
            return false;
        int level = std::stoi(nowToken->value);
        if (needMore) {
            if (level <= levels.back()) {
                throw Exception("invalid begin line");
            }
        } else {
            if (level > levels.back()) {
                throw Exception("Invalid in begin of line", nowToken->numLine,
                                nowToken->numPos);
            }
            if (levels.back() < level) {
                return false;
            }
        }
        levels.push_back(level);
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
                    throw Exception("Invalid and expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readCompOp() {
        if (nowToken->value == "<"
            || nowToken->value == ">"
            || nowToken->value == "=="
            || nowToken->value == ">="
            || nowToken->value == "<="
            || nowToken->value == "!=") {
            getToken();
            return true;
        }
        return false;
    }


    bool readExpr() {
        if (!readXorExpr())
            return false;
        while (!isEnd()) {
            if (nowToken->value == "|") {
                getToken();
                if (!readXorExpr()) {
                    throw Exception("Invalid or expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
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
            if (nowToken->value == "^") {
                getToken();
                if (!readAndExpr()) {
                    throw Exception("Invalid xor expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
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
            if (nowToken->value == "&") {
                getToken();
                if (!readShiftExpr()) {
                    throw Exception("Invalid and expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
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
            if (nowToken->value == "<<" || nowToken->value == ">>") {
                getToken();
                if (!readArithExpr()) {
                    throw Exception("Invalid shift expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
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
            if (nowToken->value == "+" || nowToken->value == "-") {
                getToken();
                if (!readTermExpr()) {
                    throw Exception("Invalid arith expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
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
            if (nowToken->value == "*" || nowToken->value == "/" ||
                nowToken->value == "%" || nowToken->value == "//") {
                getToken();
                if (!readFactorExpr()) {
                    throw Exception("Invalid arith expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readFactorExpr() {
        while (!isEnd()) {
            if (nowToken->value == "-" || nowToken->value == "+" ||
                nowToken->value == "~") {
                getToken();
                if (!(readFactorExpr() || readPowerExpr())) {
                    throw Exception("Invalid factor expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
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
            if (nowToken->value == "**") {
                getToken();
                if (!readNameExpr()) {
                    throw Exception("Invalid power expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
            } else {
                break;
            }
        }
        return true;
    }

    bool readNameExpr() {
        if (nowToken->value == "(") {
            getToken();
            if (!readTest()) {
                throw Exception("Invalid brackets statement expression",
                                nowToken->numLine, nowToken->numPos);
            }
            if (nowToken->value != ")") {
                throw Exception("Invalid brackets statement expression",
                                nowToken->numLine, nowToken->numPos);
            }
            getToken();
        } else if (nowToken->type == Token::NAME
                   || nowToken->type == Token::NUMBER
                   || nowToken->type == Token::STRING
                   || nowToken->value == "None"
                   || nowToken->value == "True"
                   || nowToken->value == "False")
            getToken();
        else
            throw Exception("Invalid name expression",
                            nowToken->numLine, nowToken->numPos);
        while (readTrailer());
        return true;
    }

    bool readTrailer() {
        if (nowToken->value == "(") {
            getToken();
            readArglist();
            if (nowToken->value == ")") {
                getToken();
                return true;
            } else
                throw Exception("Invalid trailer",
                                nowToken->numLine, nowToken->numPos);
        }
        if (nowToken->value == "[") {
            getToken();
            readTest();
            if (nowToken->value == "]") {
                getToken();
                return true;
            } else
                throw Exception("Invalid trailer",
                                nowToken->numLine, nowToken->numPos);
        }
        if (nowToken->value == ".") {
            getToken();
            if (nowToken->type != Token::NAME) {
                throw Exception("Invalid trailer",
                                nowToken->numLine, nowToken->numPos);
            } else {
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
            if (nowToken->value == ",") {
                getToken();
                if (!readTest()) {
                    throw Exception("Invalid arglist",
                                    nowToken->numLine,
                                    nowToken->numPos);
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
                                nowToken->numLine,
                                nowToken->numPos);
            }
        }
        return true;
    }

    bool readNotTest() {
        if (nowToken->value == "not") {
            getToken();
            if (!readNotTest()) {
                throw Exception("invalid not expression",
                                nowToken->numLine, nowToken->numPos);
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
            if (nowToken->value == "and") {
                getToken();
                if (!readNotTest()) {
                    throw Exception("Invalid not expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
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
        if (nowToken->value == "del") {
            getToken();
            if (!readExpr()) {
                throw Exception("invalid delete stmt",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            return true;
        }
        return false;
    }

    bool readPassStmt() {
        if (nowToken->value == "pass") {
            getToken();
            return true;
        }
        return false;
    }

    bool nowImportStmt() {
        if (nowToken->value == "import") {
            getToken();
            if (nowToken->type != Token::NAME) {
                throw Exception("invalid import stmt",
                                nowToken->numLine,
                                nowToken->numPos);
            }
            if (!isEnd()) {
                if (nowToken->value == "import") {
                    getToken();
                    if (nowToken->type == Token::NAME) {
                        getToken();
                    } else {
                        throw Exception("invalid import alias",
                                        nowToken->numLine,
                                        nowToken->numPos);
                    }
                }
            }
            return true;
        }
        return false;
    }

    bool readBreakStmt() {
        if (nowToken->value == "break") {
            getToken();
            return true;
        }
        return false;
    }

    bool readContinueStmt() {
        if (nowToken->value == "continue") {
            getToken();
            return true;
        }
        return false;
    }

    bool readReturnStmt() {
        if (nowToken->value == "return") {
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
               || readForStmt() || readFuncdef()
               || readClassdef();
    }

    bool readFuncdef() {
        if (nowToken -> value != "def")
            return false;
        getToken();
        if (nowToken -> type != Token::NAME)
            throw Exception("expected NAME after def",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (nowToken -> value != ":")
            throw Exception("expected : after function name",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!readParameters())
            throw Exception("invalid function parameters",
                                  nowToken->numLine,
                                  nowToken->numPos);
        if (nowToken -> value != ":")
            throw Exception("expected : after function parameters",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!readSuite()){
            throw Exception("invalid function suite",
                            nowToken->numLine,
                            nowToken->numPos);
        }
        return true;
    }

    bool readParameters(){
        if (nowToken -> value != "(")
            return false;
        getToken();
        if (!readFuncdefarglist())
            throw Exception("invalid arglist",
                            nowToken->numLine,
                            nowToken->numPos);
        if (nowToken -> value != ")")
            throw Exception("expected )",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        return true;
    }

    bool readFuncdefarglist(){
        if (nowToken -> type != Token::NAME)
            throw Exception("expected name",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        while (nowToken -> value == ","){
            getToken();
            if (nowToken -> type != Token::NAME)
                throw Exception("expected name",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
        }
        return true;
    }

    bool readWhileStmt() {
        if (nowToken -> value != "while")
            return false;
        getToken();
        if (!readTest())
            throw Exception("invalid while expr",
                            nowToken->numLine,
                            nowToken->numPos);
        if (nowToken -> value != ":")
            throw Exception("expected : after while condition",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!readSuite())
            throw Exception("invalid while suite",
                            nowToken->numLine,
                            nowToken->numPos);
        if (nowToken -> value == "else"){
            getToken();
            if (nowToken -> value != ":")
                throw Exception("expected : after else",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
            if (!readSuite())
                throw Exception("invalid else suite",
                                nowToken->numLine,
                                nowToken->numPos);
        }
        return true;
    }

    bool readForStmt() {
        if (nowToken -> value != "for")
            return false;
        getToken();
        if (nowToken -> NAME != Token::NAME)
            throw Exception("expected name after for",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (nowToken -> value != "in")
            throw Exception("expected in after for in",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!readTest())
            throw Exception("invalid for condition",
                            nowToken->numLine,
                            nowToken->numPos);
        if (nowToken -> value != ":")
            throw Exception("expected : after for statement",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!readSuite())
            throw Exception("invalid for suite",
                            nowToken->numLine,
                            nowToken->numPos);
        if (nowToken -> value == "else"){
            getToken();
            if (nowToken -> value != ":")
                throw Exception("expected : after else",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
            if (!readSuite())
                throw Exception("invalid else suite",
                                nowToken->numLine,
                                nowToken->numPos);
        }
        return true;
    }

    bool readSuite() {
        if (!readBeginLine(true)) {
            return false;
        }
        if (!readSimpleStmt())
            return false;
        while (readBeginLine()) {
            while (readBeginLine()) {}
            if (!readSimpleStmt())
                return false;
        }
        return readStmt();
    }

    bool readIfStmt() {
        if (nowToken->value != "if")
            return false;
        getToken();
        if (!readTest())
            throw Exception("invalid if condition",
                            nowToken->numLine,
                            nowToken->numPos);
        if (nowToken->value != ":") {
            throw Exception("expected : after if condition",
                            nowToken->numLine,
                            nowToken->numPos);
        }
        getToken();
        readSuite();
        while (nowToken->value == "elif") {
            getToken();
            if (!readTest())
                throw Exception("invalid if condition",
                                nowToken->numLine,
                                nowToken->numPos);
            if (nowToken->value != ":")
                throw Exception("expected : after elif statements",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
            readSuite();
        }
        if (nowToken->value == "else") {
            getToken();
            if (nowToken->value != ":")
                throw Exception("expected : after else statements",
                                nowToken->numLine,
                                nowToken->numPos);
            readSuite();
        }
    }

    bool readClassdef() {
        if (nowToken->value != "class")
            return false;
        getToken();
        if (nowToken->type != Token::NAME)
            throw Exception("expected class name", nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (nowToken->value != ":") {
            getToken();
            readSuite();
        }
    }

    void file_input_parse() {
        while (indexNowToken < tokens.size()) {
            readBeginLine();
            readStmt();
        }
        if (levels.size() > 1) {
            throw Exception("invalid EOF");
        }
    }

    bool isEnd() {
        return nowToken->type == Token::ENDMARKER;
    }

    void getToken() {
        if (indexNowToken + 1 < tokens.size()) {
            indexNowToken++;
            nowToken = &tokens[indexNowToken];
        }
        throw Exception("invalid EOL");
    }

    void popLevel() {
        levels.pop_back();
    }
};


#endif //WOW_SYNTAXCHECK_H
