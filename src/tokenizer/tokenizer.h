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

    const std::vector <string> keywords = {
            "if", "while", "for", "import", "break",
            "continue", "class", "return", "def",
            "xor", "or", "and", "not", "True",
            "False", "None", "del", "pass"};

    void isEnd(int pos) {
        return pos >= input.size();
    }

    void readComment(int &pos) {
        for (pos; !isEnd(pos); pos++) {
            if (input[pos] == '\n') {
                return;
            }
        }
    }

    int readBeginSpace(int &pos) {
        int cntSpace = 0;
        for (pos; !isEnd(pos); pos++, cntSpace++) {
            if (input[pos] != ' ') {
                break;
            }
        }
        return cntSpace;
    }

    void parse(std::vector <std::Token> &ans) {
        int posInLine = 0;
        int line = 0;
        int pos = 0;
        while (true) {
            if (input[pos] == ' ') {
                int cntSpace = readBeginSpace(pos);
                ans.push_back(Token(line, 0, std::to_string(cntSpace)));
            } else if (input[pos] == "#") {
                readComment(pos);
                pos++;
                if (!isEnd(pos) && input[pos] == '\n') {
                    line++;
                }
            } else if (input[pos] == '\n') {

            }
            // TODO
        }
    }

public:
    std::vector <std::Token> tokenize() {
        std::vector <std::Token> ans;
        parse(input, ans);
    }
};


#endif //WOW_TOKENIZER_H
