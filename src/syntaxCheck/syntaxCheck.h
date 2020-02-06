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

    bool readTest() {
        if (!readAndTest())
            return false;
        while (!isEnd() && tokens[nowToken].type != Token::BEGIN_LINE) {
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
        while (!isEnd() && tokens[nowToken].type != Token::BEGIN_LINE) {
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
            if (!readExpr()) {
                throw Exception("invalid pass stmt",
                                tokens[nowToken].numLine,
                                tokens[nowToken].numPos); // FIXME stmt
            }
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
        if (readSimpleStmt())
            return true;
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

    Token getToken() {
        if (nowToken + 1 < tokens.size())
            nowToken++;
        throw Exception("invalid EOL");
    }
};


#endif //WOW_SYNTAXCHECK_H
