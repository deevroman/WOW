#ifndef WOW_TOKEN_H
#define WOW_TOKEN_H

class Token{
    enum TypeToken {KEYWORD, NAME, STRING, NUMBER, BEGIN_BLOCK, END_BLOCK, OPERATOR}, type;
    int numLine, numPos;
    std::string value;
};

#endif //WOW_TOKEN_H
