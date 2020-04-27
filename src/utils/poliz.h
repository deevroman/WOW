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
        POW, COPY,
        GET_VALUE, INDEX_VALUE,
        DEF_FUNC, DEF_CLASS,
        CALL_FUNC, CREATE_CLASS,
        GET_FIELD, EVAL_METHOD,
        GET_VALUE_INT, GET_VALUE_DOUBLE, GET_VALUE_STR,
        GET_VALUE_NONE, GET_VALUE_BOOL,
        CLEAR_STACK
    } TYPE;
    bool isNotSavingToPoliz = false; // FIXME удалить и заменить на CLEAR_STACK
    int isJump = 0; // 0 - нет, иначе позиция
    int intValue = 0;
    double doubleValue = 0.0; // TODO можно переписать на объединение
    std::string stringValue = ""; // TODO на указатели сюда же имя функции
};


class Poliz {
public:
    std::unordered_map<std::string, Poliz> funcs; // TODO classes
    std::vector<Element> operations;
};

#endif //WOW_POLIZ_H