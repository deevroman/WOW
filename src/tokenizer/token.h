#ifndef WOW_TOKEN_H
#define WOW_TOKEN_H

#include "../../external/ThorsSerializer/ThorSerialize/Traits.h"

struct Token {
    enum TypeToken {
        KEYWORD, NAME, STRING,
        NUMBER, BEGIN_LINE, BEGIN_BLOCK, OPERATOR, // TODO возможно стоит разделить на NUMBER_INT и NUMBER_DOUBLE
        ENDMARKER
    };
    TypeToken type;
    int numLine, numPos;
    std::string value;
    Token(){}
    Token(TypeToken type, int numLine, int numPos, std::string value) :
            type(type), numLine(numLine), numPos(numPos), value(value) {}

    bool operator==(const Token &oth) const {
        return type == oth.type
               && numLine == oth.numLine
               && numPos == oth.numPos
               && value == oth.value;
    }

    friend class ThorsAnvil::Serialize::Traits<Token>;
};

ThorsAnvil_MakeEnum(Token::TypeToken, KEYWORD, NAME, STRING,
                    NUMBER, BEGIN_LINE, BEGIN_BLOCK, OPERATOR, ENDMARKER);
ThorsAnvil_MakeTrait(Token, type, numLine, numPos, value);

#endif //WOW_TOKEN_H
