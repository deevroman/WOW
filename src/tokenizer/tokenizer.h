#ifndef WOW_TOKENIZER_H
#define WOW_TOKENIZER_H

#include "token.h"

#include <vector>
#include <string>

class Tokenizer {
public:
    Tokenizer(const std::string &input) : input(input) {}

private:
    std::string input;

    const std::vector <std::string> keywords = {
            "if", "while", "for", "import", "break",
            "continue", "class", "return", "def",
            "xor", "or", "and", "not", "True",
            "False", "None", "del", "pass", "list"};

    const std::vector <std::string> operators = {"<", ">", "==", ">=", "<=", "!=",
                                                 "^", "&", "<<", ">>", "+", "-",
                                                 "%", "*", "**", "/", "//", "~",
                                                 "(", ")", "[", "]", ".",
                                                 "+=", "-=", "*=", "/=", "**=", "//="};

    bool isEnd(int pos) {
        return pos >= input.size();
    }

    int readBeginSpace(int &pos) {
        int cntSpace = 0;
        while (!isEnd(pos)) {
            if (input[pos] != ' ') {
                break;
            }
            pos++, cntSpace++
        }
        return cntSpace;
    }

    void readComment(int &pos) {
        for (pos; !isEnd(pos); pos++) {
            if (input[pos] == '\n') {
                return;
            }
        }
    }

    void readSpaces(int &pos, int &posInLine) {
        while (!isEnd(pos) && input[pos] != ' ') {
            pos++;
            posInLine++;
        }
    }

    bool isBeginOperator(int pos) {
        if (!isEnd(pos)) {
            if (!isEnd(pos + 1) && !isEnd(pos + 2)) {
                std::string = input.substr(pos, 3);
                for (auto now : operators) {
                    if (std::find(operators.begin(), operators.end(), now) != operators.end()) {
                        return true;
                    }
                }
            }
            if (!isEnd(pos + 1)) {
                std::string = input.substr(pos, 2);
                for (auto now : operators) {
                    if (std::find(operators.begin(), operators.end(), now) != operators.end()) {
                        return true;
                    }
                }
            }
            std::string = input.substr(pos, 1);
            for (auto now : operators) {
                if (std::find(operators.begin(), operators.end(), now) != operators.end()) {
                    return true;
                }
            }
        }
        return false;
    }

    std::string readOperator(int &pos, int &posInLine) {
        if (!isEnd(pos)) {
            if (!isEnd(pos + 1) && !isEnd(pos + 2)) {
                std::string = input.substr(pos, 3);
                for (auto now : operators) {
                    if (std::find(operators.begin(), operators.end(), now) != operators.end()) {
                        pos += 3;
                        posInLine += 3;
                        return now;
                    }
                }
            }
            if (!isEnd(pos + 1)) {
                std::string = input.substr(pos, 2);
                for (auto now : operators) {
                    if (std::find(operators.begin(), operators.end(), now) != operators.end()) {
                        pos += 2;
                        posInLine += 2;
                        return now;
                    }
                }
            }
            std::string = input.substr(pos, 1);
            for (auto now : operators) {
                if (std::find(operators.begin(), operators.end(), now) != operators.end()) {
                    pos += 1;
                    posInLine += 1;
                    return now;
                }
            }
        }
    }

    std::string readNumber(int &pos, int &posInLine) {
        std::string num;
        while (!isEnd(pos) && (input[pos] >= '0' && input[pos] <= '9')) {
            num += input[pos];
            pos++;
            posInLine++;
        }
        return num;
    }

    std::string readString(int &pos, int &posInLine, int &line) {
        std::string str;
        if (input[pos] == '\'') {
            pos++;
            posInLine++;
        }
        while (!isEnd(pos)) {
            if (input[pos] == '\'') {
                if (isEnd(pos + 1)) {
                    // TODO throw
                    std::cerr << "Invalid EOF";
                    return "";
                } else if (input[pos + 1] == '\'') {
                    str += '\'';
                    pos += 2;
                    posInLine += 2;
                } else if (input[pos + 1] == '\\') {
                    str += '\\';
                    pos += 2;
                    posInLine += 2;
                } else if (input[pos + 1] == '\n') {
                    line++;
                    pos += 2;
                    posInLine = 0;
                } else {
                    std += '\\';
                }
            } else if (input[pos] == '\n') {
                line++;
                pos++;
                posInLine = 0;
                str += '\n';
            } else if (input[pos] == '\'') {
                pos += 2;
                posInLine += 2;
                break;
            } else {
                str += input[pos];
                pos++;
                posInLine++;
            }
        }
        return str;
    }

    bool isKeyword(int pos) {
        // TODO
    }

    std::string readKeyword(int &pos, int &posInLine) {

    }

    void parse(std::vector <std::Token> &ans) {
        int posInLine = 0;
        int line = 0;
        int pos = 0;
        while (!isEnd(pos)) {
            int cntSpace = readBeginSpace(pos);
            ans.push_back(Token(Token::BEGIN_BLOCK, line, 0, std::to_string(cntSpace)));
            posInLine = cntSpace;

            parseLine(pos, line, posInLine, ans);
            line++;
            posInLine = 0;
        }
    }

    void parseLine(int &pos, int &line, int &posInLine, std::vector <std::Token> &ans) {
        while (!isEnd(pos) && input[pos] != '\n') {
            if (input[pos] == '#') {
                readComment(pos, posInLine);
            } else if (isBeginOperator(pos)) {
                std::string op = readOperator(pos, posInLine);
                ans.push_back(Token(Token::OPERATOR, line, posInLine, op));
            } else if (input[pos] >= '0' && input[pos] <= '9') {
                std::string num = readNumber(pos, posInLine);
                ans.push_back(Token(Token::NUMBER, line, posInLine, num));
            } else if (input[pos] == '\'') {
                std::string str = readString(pos, posInLine, line);
                ans.push_back(Token(Token::STRING, line, posInLine, str));
            } else if (input[pos] == ' ') {
                readSpaces(pos, posInLine);
            } else if (isKeyword(pos)) {
                std::string keyword = readKeyword(pos, posInLine);
                ans.push_back(Token(Token::KEYWORD, line, posInLine, keyword));
            } else if (input[pos] >= "a" && input[pos] <= 'z'
                       || input[pos] >= 'A' && input[pos] <= 'Z' || input[pos] == '_') {
                std::string name = readName(pos, posInLine);
                ans.push_back(Token(Token::NAME, line, posInLine, name));
            } else {
                // TODO throw
                std::cerr << "Invalid //TODO";
                return;
            }
        }
    }

public:

    std::vector <std::Token> tokenize() {
        std::vector <std::Token> ans;
        parse(input, ans);
    }

};


#endif //WOW_TOKENIZER_H
