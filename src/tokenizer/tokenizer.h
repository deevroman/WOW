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

    const std::vector<std::string> keywords = {
            "if", "while", "for", "else", "elif", "import", "break",
            "continue", "class", "return", "def",
            "xor", "or", "and", "not", "in", "True",
            "False", "None", "del", "pass", "list"};

    const std::vector<std::string> operators = {"<", ">", "==", ">=", "<=", "!=",
                                                "^", "&", "<<", ">>", "+", "-",
                                                "%", "*", "**", "/", "//", "~",
                                                "(", ")", "[", "]", ".", ",",
                                                "+=", "-=", "*=", "/=", "%=", "**=", "//=", "=",
                                                "&=", "|=", "^=", "<<=", ">>="};

    class Exception {
    public:
        int code;
        std::string message;

        Exception(int code, std::string message) :
                code(code), message(message) {}
    };

    bool isEnd(int pos) {
        return pos >= input.size();
    }

    int readBeginSpace(int &pos) {
        int cntSpace = 0;
        while (!isEnd(pos)) {
            if (input[pos] != ' ') {
                break;
            }
            pos++, cntSpace++;
        }
        return cntSpace;
    }

    void readComment(int &pos) {
        while (!isEnd(pos)) {
            if (input[pos] == '\n') {
                return;
            }
            pos++;
        }
    }

    void readSpaces(int &pos, int &posInLine) {
        while (!isEnd(pos) && input[pos] == ' ') {
            pos++;
            posInLine++;
        }
    }

    bool isBeginOperator(int pos) {
        if (!isEnd(pos)) {
            if (!isEnd(pos + 1) && !isEnd(pos + 2)) {
                std::string subStr = input.substr(pos, 3);
                if (find(operators.begin(), operators.end(), subStr) != operators.end()) {
                    return true;
                }
            }
            if (!isEnd(pos + 1)) {
                std::string subStr = input.substr(pos, 2);
                if (find(operators.begin(), operators.end(), subStr) != operators.end()) {
                    return true;
                }
            }
            std::string subStr = input.substr(pos, 1);
            if (find(operators.begin(), operators.end(), subStr) != operators.end()) {
                return true;
            }
        }
        return false;
    }

    std::string readOperator(int &pos, int &posInLine) {
        if (!isEnd(pos)) {
            if (!isEnd(pos + 1) && !isEnd(pos + 2)) {
                std::string subStr = input.substr(pos, 3);
                if (find(operators.begin(), operators.end(), subStr) != operators.end()) {
                    pos += 3;
                    posInLine += 3;
                    return subStr;
                }
            }
            if (!isEnd(pos + 1)) {
                std::string subStr = input.substr(pos, 2);
                if (find(operators.begin(), operators.end(), subStr) != operators.end()) {
                    pos += 2;
                    posInLine += 2;
                    return subStr;
                }
            }
            std::string subStr = input.substr(pos, 1);
            if (find(operators.begin(), operators.end(), subStr) != operators.end()) {
                pos += 1;
                posInLine += 1;
                return subStr;
            }
        }
        throw Exception(-1, "Bred");
    }

    std::string readNumber(int &pos, int &posInLine) {
        std::string num;
        int cntPoints = 0;
        while (!isEnd(pos)
               && ((input[pos] >= '0' && input[pos] <= '9') || input[pos] == '.')) {
            if (input[pos] == '.') {
                if (cntPoints == 0) {
                    cntPoints++;
                } else {
                    throw Exception(3, "Invalid float number format");
                }
            }
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
        bool ended = false;
        while (!isEnd(pos)) {
            if (input[pos] == '\\') {
                if (isEnd(pos + 1)) {
                    throw Exception(1, "Invalid EOF");
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
                    str += '\\';
                }
            } else if (input[pos] == '\n') {
                line++;
                pos++;
                posInLine = 0;
                str += '\n';
            } else if (input[pos] == '\'') {
                pos += 1;
                posInLine += 1;
                ended = true;
                break;
            } else {
                str += input[pos];
                pos++;
                posInLine++;
            }
        }
        if(!ended)
            throw Exception(1, "Invalid EOF");
        return str;
    }

    bool isKeyword(int pos) {
        for (const auto &now : keywords) {
            if (!isEnd(pos + (int) now.size() - 1)) {
                int indexNext = pos + (int) now.size();
                if (isEnd(indexNext)
                    || ((input[indexNext] < 'a' || input[indexNext] > 'z')
                        && (input[indexNext] < 'A' || input[indexNext] > 'Z')
                        && input[indexNext] != '_')) {
                    if (now == input.substr(pos, now.size())) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    std::string readKeyword(int &pos, int &posInLine) {
        for (const auto &now : keywords) {
            if (!isEnd(pos + (int) now.size() - 1)) {
                int indexNext = pos + (int) now.size();
                if (isEnd(indexNext)
                    || ((input[indexNext] < 'a' || input[indexNext] > 'z')
                        && (input[indexNext] < 'A' || input[indexNext] > 'Z')
                        && input[indexNext] != '_')) {
                    if (now == input.substr(pos, now.size())) {
                        pos += now.size();
                        posInLine += now.size();
                        return now;
                    }
                }
            }
        }
        throw Exception(-1, "Bred");
    }

    std::string readName(int &pos, int &posInLine) {
        std::string name = {input[pos]};
        pos++;
        posInLine++;
        while (!isEnd(pos) &&
               ((input[pos] >= 'a' && input[pos] <= 'z')
                || (input[pos] >= 'A' && input[pos] <= 'Z')
                || (input[pos] >= '0' && input[pos] <= '9')
                || input[pos] == '_')) {
            name += input[pos];
            pos++;
            posInLine++;
        }
        return name;
    }

    void parse(std::vector<Token> &ans) {
        int posInLine = 0;
        int line = 0;
        int pos = 0;
        try {
            while (!isEnd(pos)) {
                int cntSpace = readBeginSpace(pos);
                ans.push_back(Token(Token::BEGIN_LINE, line, 0, std::to_string(cntSpace)));
                posInLine = cntSpace;

                parseLine(pos, line, posInLine, ans);
                line++;
                posInLine = 0;
            }
        } catch (Exception e) {
            throw std::to_string(line + 1) + ":" + std::to_string(posInLine + 1) + "\n"
                  + e.message;
        }
    }

    void parseLine(int &pos, int &line, int &posInLine, std::vector<Token> &ans) {
        while (!isEnd(pos)) {
            if (input[pos] == '\n') {
                pos++;
                break;
            } else if (input[pos] == '#') {
                readComment(pos);
            } else if (isBeginOperator(pos)) {
                int startPosInLine = posInLine;
                std::string op = readOperator(pos, posInLine);
                ans.push_back(Token(Token::OPERATOR, line, startPosInLine, op));
            } else if (input[pos] >= '0' && input[pos] <= '9') {
                int startPosInLine = posInLine;
                std::string num = readNumber(pos, posInLine);
                ans.push_back(Token(Token::NUMBER, line, startPosInLine, num));
            } else if (input[pos] == '\'') {
                int startPosInLine = posInLine;
                int startLine = line;
                std::string str = readString(pos, posInLine, line);
                ans.push_back(Token(Token::STRING, startLine, startPosInLine, str));
            } else if (input[pos] == ' ') {
                readSpaces(pos, posInLine);
            } else if (isKeyword(pos)) {
                int startPosInLine = posInLine;
                std::string keyword = readKeyword(pos, posInLine);
                ans.push_back(Token(Token::KEYWORD, line, startPosInLine, keyword));
            } else if ((input[pos] >= 'a' && input[pos] <= 'z')
                       || (input[pos] >= 'A' && input[pos] <= 'Z') || input[pos] == '_') {
                int startPosInLine = posInLine;
                std::string name = readName(pos, posInLine);
                ans.push_back(Token(Token::NAME, line, startPosInLine, name));
            } else if (input[pos] == ':') {
                ans.push_back(Token(Token::BEGIN_BLOCK, line, posInLine, ":"));
                pos++;
                posInLine++;
            } else {
                throw Exception(2, "Invalid syntax");
            }
        }
    }

public:

    std::vector<Token> tokenize() {
        std::vector<Token> ans;
        parse(ans);
        return ans;
    }


};


#endif //WOW_TOKENIZER_H
