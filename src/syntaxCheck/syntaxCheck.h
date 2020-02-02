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
                    throw Exception( "invalid begin line");
                }
            } else {
                while (levels.back() <= level) {
                    levels.pop_back();
                }
            }
            levels.push_back(level);
        } else {
            throw Exception( "invalid begin line");
        }
    }

    bool nextTokenIsNewline(){
        return nowToken + 1 < tokens.size() && tokens[nowToken+1].type == Token::BEGIN_LINE;
    }

    void readStmt() {

    }

    void file_input_parse() {
        while (nowToken < tokens.size()) {
            readBeginLine();
            readStmt();
        }
        if(levels.size() > 1){
            throw Exception( "invalid EOF");
        }
    }
};


#endif //WOW_SYNTAXCHECK_H
