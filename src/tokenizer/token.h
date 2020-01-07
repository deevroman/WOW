#ifndef WOW_TOKEN_H
#define WOW_TOKEN_H

struct Token {
    enum TypeToken {
        KEYWORD, NAME, STRING,
        NUMBER, BEGIN_BLOCK, OPERATOR
    } type;
    int numLine, numPos;
    std::string value;

    Token(TypeToken type, int numLine, int numPos, std::string value) :
            type(type), numLine(numLine), numPos(numPos), value(value) {}
};

#endif //WOW_TOKEN_H
