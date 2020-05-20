#ifndef WOW_TOKEN_H
#define WOW_TOKEN_H

struct Token {
    enum TypeToken {
        KEYWORD, NAME, STRING,
        NUMBER, BEGIN_LINE, BEGIN_BLOCK, OPERATOR, // TODO возможно стоит разделить на NUMBER_INT и NUMBER_DOUBLE
        ENDMARKER
    };
    TypeToken type;
    int numLine, numPos;
    std::string value;

    Token() {}

    Token(TypeToken type, int numLine, int numPos, std::string value) :
            type(type), numLine(numLine), numPos(numPos), value(value) {}

    bool operator==(const Token &oth) const {
        return type == oth.type
               && numLine == oth.numLine
               && numPos == oth.numPos
               && value == oth.value;
    }

};


#endif //WOW_TOKEN_H
