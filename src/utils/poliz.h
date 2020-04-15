#ifndef WOW_POLIZ_H
#define WOW_POLIZ_H

#include <vector>
#include <string>
#include <unordered_map>

class Element {
public:
    int countParams = 0; // TODO возможно можно перенести
    enum PrimeFuncs {
        OR, AND, NOT,
        XOR, OR_BIT,
        AND_BIT, SHIFT_LEFT_BIN, SHIFT_RIGHT_BIN,
        PLUS, MINUS, MUL, DIV, MOD, INTDIV,
        UNAR_PLUS, UNAR_MINUS, UNAR_TILDA,
        POW,
        GET_VALUE, GET_VALUE_INDEX, SET_VALUE_INDEX,
        FUNCTION, GET_FIELD, EVAL_METHOD,
        GET_VALUE_INT, GET_VALUE_DOUBLE, GET_VALUE_STR,
        GET_VALUE_NONE, GET_VALUE_BOOL
    } TYPE;
    bool isNotSavingToPoliz = false;
    int isJump = 0; // 0 - нет, иначе позиция
    int intValue = 0;
    double doubleValue = 0.0; // TODO переписать на объединение
    std::string stringValue = ""; // TODO на указатели сюда же имя функции
};


class Poliz {
public:
    std::unordered_map<std::string, Poliz> funcs; // TODO classes
    std::vector<Element> operations;
};

#endif //WOW_POLIZ_H
