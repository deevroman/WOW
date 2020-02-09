#ifndef WOW_SYNTAXCHECK_H
#define WOW_SYNTAXCHECK_H

#include <vector>
#include "../tokenizer/tokenizer.h"

class SyntaxCheck {
    std::vector<Token> tokens;
public:
    explicit SyntaxCheck(std::vector<Token> &v) : tokens(v) {
        tokens.push_back(Token(Token::ENDMARKER, tokens.back().numLine, tokens.back().numPos + 1, ""));
    }

    void check() {
        indexNowToken = 0;
        nowToken = &tokens[0];
        levels = {0};
        try {
            readFileInput();
        } catch (Exception e) {
            std::string errorMessage;
            if (e.line != -1 && e.posInLine != -1) {
                errorMessage += std::to_string(e.line + 1) + ":" + std::to_string(e.posInLine + 1) + " ";
            }
            errorMessage += e.message;
            throw errorMessage;
        }
    }

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
                throw Exception("invalid in begin of line", nowToken->numLine,
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
            } else {
                break;
            }
        }
        return true;
    }

    bool readCompOp() {
        if (isOperator("<") || isOperator(">")
            || isOperator("==") || isOperator(">=")
            || isOperator("<=") || isOperator("!=")) {
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
            if (isOperator("^")) {
                getToken();
                if (!readAndExpr()) {
                    throw Exception("invalid xor expression",
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
            if (isOperator("&")) {
                getToken();
                if (!readShiftExpr()) {
                    throw Exception("invalid and expression",
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
            if (isOperator("<<") || isOperator(">>")) {
                getToken();
                if (!readArithExpr()) {
                    throw Exception("invalid shift expression",
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
            if (isOperator("+") || isOperator("-")) {
                getToken();
                if (!readTermExpr()) {
                    throw Exception("invalid arith expression",
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
            if (isOperator("*") || isOperator("/") ||
                isOperator("%") || isOperator("//")) {
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
        if (!isEnd()) {
            if (isOperator("-")
                || isOperator("+")
                || isOperator("~")) {
                getToken();
                if (!readFactorExpr()) {
                    throw Exception("invalid factor expression",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
                return true;
            } else {
                return readPowerExpr();
            }
        } else {
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
            } else {
                break;
            }
        }
        return true;
    }

    bool readNameExpr() {
        if (isOperator("[")) {
            getToken();
            if (!readTest()){
                if (isOperator("]")) {
                    getToken();
                    while(readTrailer());
                    return true;
                }
            }
            if (isOperator(",")) {
                while (isOperator(",")) {
                    getToken();
                    if (!readTest()) {
                        throw Exception("expected testExpr",
                                        nowToken->numLine, nowToken->numPos);
                    }
                }
            } else if (isKeyword("for")) {
                getToken();
                if (nowToken->type != Token::NAME)
                    throw Exception("expected NAME",
                                    nowToken->numLine, nowToken->numPos);
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
        } else if (nowToken->type == Token::NAME
                   || nowToken->type == Token::NUMBER
                   || nowToken->type == Token::STRING
                   || isKeyword("None")
                   || isKeyword("True")
                   || isKeyword("False"))
            getToken();
        else
            return false;
        while (readTrailer());
        return true;
    }

    bool readTrailer() {
        if (isOperator("(")) {
            getToken();
            readArglist();
            if (isOperator(")")) {
                getToken();
                return true;
            } else
                throw Exception("invalid trailer",
                                nowToken->numLine, nowToken->numPos);
        }
        if (isOperator("[")) {
            getToken();
            readTest();
            if (isOperator("]")) {
                getToken();
                return true;
            } else
                throw Exception("invalid trailer",
                                nowToken->numLine, nowToken->numPos);
        }
        if (isOperator(".")) {
            getToken();
            if (nowToken->type != Token::NAME) {
                throw Exception("invalid trailer",
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
            if (isOperator(",")) {
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
        if (isKeyword("not")) {
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
            if (isKeyword("and")) {
                getToken();
                if (!readNotTest()) {
                    throw Exception("invalid not expression",
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
            getToken();
            if (!isEnd()) {
                if (isKeyword("as")) {
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
        if (isKeyword("break")) {
            getToken();
            return true;
        }
        return false;
    }

    bool readExprStmt() {
        if (!readTest())
            return false;
        if (readAugassign()) {
            if (!readTest())
                throw Exception("invalid test",
                                nowToken->numLine,
                                nowToken->numPos);
        } else {
            while (isOperator("=")) {
                getToken();
                if (!readTest()) {
                    throw Exception("invalid test expr",
                                    nowToken->numLine,
                                    nowToken->numPos);
                }
            }
        }
        return true;
    }

    bool readAugassign() {
        if (isOperator("+=") || isOperator("-=")
            || isOperator("*=") || isOperator("/=")
            || isOperator("%=") || isOperator("**=")
            || isOperator("&=") || isOperator("|=")
            || isOperator("^=") || isOperator("<<=")
            || isOperator(">>=")) {
            getToken();
            return true;
        }
        return false;
    }

    bool readContinueStmt() {
        if (isKeyword("continue")) {
            getToken();
            return true;
        }
        return false;
    }

    bool readReturnStmt() {
        if (isKeyword("return")) {
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
               || readImportStmt() || readFlowStmt()
               || readExprStmt();
    }

    bool readCompoundStmt() {
        return readIfStmt() || readWhileStmt()
               || readForStmt() || readFuncdef()
               || readClassdef();
    }

    bool readFuncdef() {
        if (nowToken->value != "def")
            return false;
        getToken();
        if (nowToken->type != Token::NAME)
            throw Exception("expected NAME after def",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!readParameters())
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
        return true;
    }

    bool readParameters() {
        if (nowToken->value != "(")
            return false;
        getToken();
        if (!readFuncdefarglist())
            throw Exception("invalid arglist",
                            nowToken->numLine,
                            nowToken->numPos);
        if (nowToken->value != ")")
            throw Exception("expected )",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        return true;
    }


    bool readFuncdefarglist() {
        if (nowToken->type != Token::NAME)
            throw Exception("expected name",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        while (isOperator(",")) {
            getToken();
            if (nowToken->type != Token::NAME)
                throw Exception("expected name",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
        }
        return true;
    }

    bool readWhileStmt() {
        if (nowToken->value != "while")
            return false;
        getToken();
        if (!readTest())
            throw Exception("invalid while condition",
                            nowToken->numLine,
                            nowToken->numPos);
        if (!isBeginBlock(":"))
            throw Exception("invalid while condition",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        readSuite();
        if (isEqualLevel() && isNextTokenKey("else")) {
            readBeginLine();
            getToken();
            if (!isBeginBlock(":"))
                throw Exception("expected : after else",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
            readSuite();
        }
        return true;
    }

    bool readForStmt() {
        if (nowToken->value != "for")
            return false;

        getToken();
        if (nowToken->NAME != Token::NAME)
            throw Exception("expected name after for",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!isKeyword("in"))
            throw Exception("expected in after for in",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!readTest())
            throw Exception("invalid for condition",
                            nowToken->numLine,
                            nowToken->numPos);
        if (!isBeginBlock(":"))
            throw Exception("expected : after for statement",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (!readSuite())
            throw Exception("invalid for suite",
                            nowToken->numLine,
                            nowToken->numPos);
        if (isEqualLevel() && isNextTokenKey("else")) {
            readBeginLine();
            getToken();
            if (!isBeginBlock(":"))
                throw Exception("expected : after else",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
            readSuite();
        }
        return true;
    }

    bool readSuite() {
        if (!readBeginLine(true)) {
            throw Exception("expected suite", nowToken->numLine, nowToken->numPos);
        }
        if (!readStmt()) {
            throw Exception("expected suite", nowToken->numLine, nowToken->numPos);
        }
        while (readBeginLine()) {
            if (!readStmt()) {
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
        if (!readTest())
            throw Exception("invalid if condition",
                            nowToken->numLine,
                            nowToken->numPos);
        if (!isBeginBlock(":")) {
            throw Exception("invalid if condition",
                            nowToken->numLine,
                            nowToken->numPos);
        }
        getToken();
        readSuite();
        while (isEqualLevel() && isNextTokenKey("elif")) {
            readBeginLine();
            getToken();
            if (!readTest())
                throw Exception("invalid elif condition",
                                nowToken->numLine,
                                nowToken->numPos);
            if (!isBeginBlock(":"))
                throw Exception("invalid elif condition",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
            readSuite();
        }
        if (isEqualLevel() && isNextTokenKey("else")) {
            readBeginLine();
            getToken();
            if (!isBeginBlock(":"))
                throw Exception("expected : after else statements",
                                nowToken->numLine,
                                nowToken->numPos);
            getToken();
            readSuite();
        }
        return true;
    }

    bool readClassdef() {
        if (!isKeyword("class"))
            return false;
        getToken();
        if (nowToken->type != Token::NAME)
            throw Exception("expected class name",
                            nowToken->numLine,
                            nowToken->numPos);
        getToken();
        if (isBeginBlock(":")) {
            getToken();
            if (!readSuite()) {
                throw Exception("Invalid class suite",
                                nowToken->numLine,
                                nowToken->numPos);
            }
        }
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
        } else {
            throw Exception("invalid EOF");
        }
    }

    bool isNextTokenKey(std::string name) {
        return (indexNowToken + 1 < tokens.size()
                && tokens[indexNowToken + 1].type == Token::KEYWORD
                && tokens[indexNowToken + 1].value == name);
    }

    bool isEqualLevel() {
        if (nowToken->type == Token::ENDMARKER)
            return levels.back() == 0;
        if (nowToken->type != Token::BEGIN_LINE)
            throw Exception("Ban");
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


#endif //WOW_SYNTAXCHECK_H
