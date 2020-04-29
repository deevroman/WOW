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
        POSITIVE_JMP, NEGATIVE_JMP, JMP,
        AND_BIT, SHIFT_LEFT_BIN, SHIFT_RIGHT_BIN,
        PLUS, MINUS, MUL, DIV, MOD, INTDIV,
        UNAR_PLUS, UNAR_MINUS, UNAR_TILDA,
        POW, COPY,
        GET_VALUE, INDEX_VALUE,
        GET_VALUES_FROM_PREV_STACK, // для вызовы функции. После def func кладём этот элемент c intValue =кол-ву аргументов, а потом copy, clear_stack для каждого аргумента
        DEF_FUNC, DEF_CLASS, // def func нужно передать аргументы
        CALL_FUNC, CREATE_CLASS,
        GET_FIELD, EVAL_METHOD,
        GET_VALUE_INT, GET_VALUE_DOUBLE, GET_VALUE_STR,
        GET_VALUE_NONE, GET_VALUE_BOOL,
        CLEAR_STACK,
        BEGIN_SCOPE,
        END_SCOPE,
        CMP_EQUAL, CMP_NOT_EQUAL, CMP_LESS, CMP_MORE, CMP_MORE_EQUAL, CMP_LESS_EQUAL,
        JUMP_TO_SCOPE, // в intValue храним индекс в массиве otherScopes
        RETURN_VALUE
    } TYPE;
    int posJump = 0;
    int intValue = 0;
    double doubleValue = 0.0; // TODO можно переписать на объединение
    std::string stringValue = ""; // сюда же имя функции
};


class Poliz {
public:
    std::map<std::string, Poliz *> funcs;
    std::map<std::string, Poliz *> classes;
    std::vector<Poliz *> otherScopes;
    std::vector<Element> operations;
};

#endif //WOW_POLIZ_H
