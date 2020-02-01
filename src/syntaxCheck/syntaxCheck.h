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
        levels = {-1};
        file_input_parse();
    }

private:
    std::vector<int> levels;
    int nowToken{};

    void readBeginLine(int needMore = false) {
        if (tokens[nowToken].type == Token::BEGIN_LINE) {
            int level = std::stoi(tokens[nowToken].value);
            if (needMore) {
                if (level <= levels.back()) {
                    throw Exception(1, "invalid begin line");
                }
            } else {
                while (levels.back() <= level) {
                    levels.pop_back();
                }
            }
            levels.push_back(level);

        } else {
            throw Exception(1, "invalid begin line");
        }
    }

    void readStmt() {

    }

    void file_input_parse() {
        while (nowToken < tokens.size()) {
            readBeginLine();
            readStmt();
        }
    }
};


#endif //WOW_SYNTAXCHECK_H
