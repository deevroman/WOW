
#ifndef WOW_VM_H
#define WOW_VM_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include "../utils/poliz.h"
#include "../utils/wowobj.h"

class VM {
private:
    struct scope {
        std::map<std::string, Poliz *> funcs;
        std::map<std::string, Poliz *> classes;
        std::map<std::string, wowobj *> vars;
        enum {
            SIMPLE, FUNC, CLASS
        } type = SIMPLE;
    };

    class Level {
    public:
        std::string name;
        Poliz *poliz;
    };

    std::vector<wowobj *> curStack;
    std::vector<Level> stackTrace;
    std::vector<scope> bigScopes;
    std::vector<scope> scopes;

    wowobj *getItemOfCurStack(int number = 0) {
        return curStack[curStack.size() - 1 - number];
    }

    std::map<std::string, wowobj *> runLevel() {
        auto &curLevel = stackTrace.back();
        int i = 0;
        for (i; i < stackTrace.back().poliz->operations.size(); i++) {
            auto &curOp = stackTrace.back().poliz->operations[i];
            switch (curOp.TYPE) {
                case Element::OR: {
                    if (getItemOfCurStack(1)->type == wowobj::BOOL
                        && getItemOfCurStack(0)->type == wowobj::BOOL) {
                        bool value = *(static_cast<bool *>(getItemOfCurStack(1)->value)) or
                                     *(static_cast<bool *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::AND: {
                    if (getItemOfCurStack(1)->type == wowobj::BOOL
                        && getItemOfCurStack(0)->type == wowobj::BOOL) {
                        bool value = *(static_cast<bool *>(getItemOfCurStack(1)->value)) and
                                     *(static_cast<bool *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::NOT: {
                    if (getItemOfCurStack(1)->type == wowobj::BOOL
                        && getItemOfCurStack(0)->type == wowobj::BOOL) {
                        bool value = not*(static_cast<bool *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::XOR: {
                    if (getItemOfCurStack(1)->type == wowobj::BOOL
                        && getItemOfCurStack(0)->type == wowobj::BOOL) {
                        bool value = *(static_cast<bool *>(getItemOfCurStack(1)->value)) xor
                                     *(static_cast<bool *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::OR_BIT: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) |
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::AND_BIT: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) &
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::SHIFT_LEFT_BIN: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) <<
                                                                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::SHIFT_RIGHT_BIN: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) >>
                                                                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::PLUS: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) +
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    if (getItemOfCurStack(1)->type == wowobj::DOUBLE
                        && getItemOfCurStack(0)->type == wowobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) +
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::DOUBLE, new double(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(1)->type == wowobj::STRING
                             && getItemOfCurStack()->type == wowobj::STRING) {
                        std::string value = (*(static_cast<std::string *>(getItemOfCurStack(1)->value)))
                                            + (*(static_cast<std::string *>(getItemOfCurStack()->value)));
                        auto tmpobj = new wowobj(wowobj::STRING, new std::string(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::MINUS: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) -
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(1)->type == wowobj::DOUBLE
                             && getItemOfCurStack(0)->type == wowobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) -
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new double(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::MUL: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) *
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(1)->type == wowobj::DOUBLE
                             && getItemOfCurStack(0)->type == wowobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) *
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new double(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                }
                case Element::DIV: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        double value = (double) *(static_cast<int *>(getItemOfCurStack(1)->value)) /
                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::DOUBLE, new double(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(1)->type == wowobj::DOUBLE
                             && getItemOfCurStack(0)->type == wowobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) /
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new double(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::MOD: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) %
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::INTDIV: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) /
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::UNAR_PLUS: {
                    if (getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::UNAR_MINUS: {
                    if (getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = -*(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(0)->type == wowobj::DOUBLE) {
                        double value = -*(static_cast<double *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::DOUBLE, new double(value));
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::UNAR_TILDA: {
                    if (getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = ~*(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::POW: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = pow(*(static_cast<int *>(getItemOfCurStack(1)->value)),
                                        *(static_cast<int *>(getItemOfCurStack()->value)));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw "invalid type for operation";
                    }
                    break;
                }
                case Element::COPY: {
                    if (getItemOfCurStack()->type == wowobj::INT
                        || getItemOfCurStack()->type == wowobj::DOUBLE
                        || getItemOfCurStack()->type == wowobj::STRING) {
                        if (getItemOfCurStack()->type == wowobj::INT) {
                            getItemOfCurStack(1)->value = static_cast<void *>(new int(
                                    *static_cast<int *>(getItemOfCurStack()->value)));
                        }
                        else if (getItemOfCurStack()->type == wowobj::STRING) {
                            getItemOfCurStack(1)->value = static_cast<void *>(new std::string(
                                    *static_cast<std::string *>(getItemOfCurStack()->value)));
                        }
                        else if (getItemOfCurStack()->type == wowobj::DOUBLE) {
                            getItemOfCurStack(1)->value = static_cast<void *>(new double(
                                    *static_cast<double *>(getItemOfCurStack()->value)));
                        }
                        getItemOfCurStack(1)->type = getItemOfCurStack()->type;
                        curStack.pop_back();
                    }
                    break;
                }
                case Element::GET_VALUE: {
                    if (bigScopes.back().vars.count(curOp.stringValue)) {
                        curStack.push_back(bigScopes.back().vars[curOp.stringValue]);
                    }
                    else {
                        curStack.push_back(new wowobj(wowobj::NONE));
                        bigScopes.back().vars[curOp.stringValue] = scopes.back().vars[curOp.stringValue] = curStack.back();
                    }
                    break;
                }
                case Element::INDEX_VALUE: {
                    if (getItemOfCurStack()->type == wowobj::INT) {
                        int ind = *static_cast<int *>(getItemOfCurStack()->value);
                        if (getItemOfCurStack(1)->type == wowobj::LIST) {
                            auto tmp = (*static_cast<std::vector<wowobj *> *>(getItemOfCurStack(1)->value))[ind];
                            curStack.push_back(tmp);
                        }
                    }
                    break;
                }
                case Element::DEF_FUNC: {
                    bigScopes.back().funcs[curOp.stringValue] = scopes.back().funcs[curOp.stringValue] = curLevel.poliz->funcs[curOp.stringValue];
                    break;
                }
                case Element::DEF_CLASS: {
                    bigScopes.back().classes[curOp.stringValue] = scopes.back().classes[curOp.stringValue] = curLevel.poliz->classes[curOp.stringValue];
                    break;
                }
                case Element::CALL_FUNC: {
                    if (curOp.stringValue == "input") {
                        auto *tmp = new std::string;
                        std::cin >> *tmp;
                        curStack.push_back(new wowobj(wowobj::STRING, static_cast<void *>(tmp)));
                    }
                    else if (curOp.stringValue == "print") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::STRING) {
                            std::cout << *(static_cast<std::string *>(arg->value));
                            curStack.push_back(new wowobj(wowobj::NONE));
                        }
                        else if (arg->type == wowobj::INT) {
                            std::cout << *(static_cast<int *>(arg->value));
                            curStack.push_back(new wowobj(wowobj::NONE));
                        }
                        else if (arg->type == wowobj::DOUBLE) {
                            std::cout << *(static_cast<double *>(arg->value));
                            curStack.push_back(new wowobj(wowobj::NONE));
                        }
                        else if (arg->type == wowobj::BOOL) {
                            std::cout << (*(static_cast<bool *>(arg->value)) ? "True" : "False");
                            curStack.push_back(new wowobj(wowobj::NONE));
                        }
                        else {
                            throw "no printble type";
                        }
                    }
                    else if (curOp.stringValue == "int") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::STRING) {
                            int *tmp = new int(std::stoi(*(static_cast<std::string *>(arg->value))));
                            curStack.push_back(new wowobj(wowobj::INT, static_cast<void *>(tmp)));
                        }
                        else if (arg->type == wowobj::DOUBLE) {
                            int *tmp = new int((int) *(static_cast<double *>(arg->value)));
                            curStack.push_back(new wowobj(wowobj::INT, static_cast<void *>(tmp)));
                        }
                        else {
                            throw "not number type";
                        }
                    }
                    else if (curOp.stringValue == "str") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::INT) {
                            auto *tmp = new std::string(std::to_string(*(static_cast<int *>(arg->value))));
                            curStack.push_back(new wowobj(wowobj::STRING, static_cast<void *>(tmp)));
                        }
                        else if (arg->type == wowobj::BOOL) {
                            auto *tmp = new std::string(*(static_cast<bool *>(arg->value)) ? "True" : "False");
                            curStack.push_back(new wowobj(wowobj::STRING, static_cast<void *>(tmp)));
                        }
                        else {
                            throw "not strinble type";
                        }
                    }
                    else if (curOp.stringValue == "list") {
                        curStack.push_back(
                                new wowobj(wowobj::LIST, static_cast<void *>(new std::vector<wowobj *>)));
                    }
                    else if (curOp.stringValue == "len") {
                        if (getItemOfCurStack()->type == wowobj::LIST) {
                            int sz = (*(static_cast<std::vector<wowobj *> *>(getItemOfCurStack()->value))).size();
                            curStack.push_back(new wowobj(wowobj::INT, new int(sz)));
                        }
                        else if (getItemOfCurStack()->type == wowobj::STRING) {
                            int sz = (*(static_cast<std::string *>(getItemOfCurStack()->value))).size();
                            curStack.push_back(new wowobj(wowobj::INT, new int(sz)));
                        }
                        else {
                            throw "not countable var";
                        }
                    }
                    else {
                        if (bigScopes.back().funcs.count(curOp.stringValue)) {
                            stackTrace.push_back({curOp.stringValue, scopes.back().funcs[curOp.stringValue]});
                            bigScopes.push_back(scopes.back());
                            bigScopes.back().type = scope::FUNC;
                            scopes.push_back({{}, {}, {}, scope::FUNC});
                            runLevel();
                        }
                        else {
                            throw "Called undefined function";
                        }
                    }
                    break;
                }
                case Element::CREATE_CLASS: {
                    bigScopes.push_back({{}, {}, {}, scope::CLASS});
                    scopes.push_back({});
                    auto fields = runLevel();
                    fields["__name__"] = new wowobj(wowobj::STRING, getItemOfCurStack()->value);
                    curStack.pop_back();
                    curStack.push_back(
                            new wowobj(wowobj::USER_CLASS,
                                       static_cast<void *>(new wowobj(wowobj::USER_CLASS,
                                                                      static_cast<void *>(new std::map<std::string, wowobj *>(
                                                                              fields))))));

                    break;
                }
                case Element::GET_FIELD: {
                    auto tmp = *static_cast<std::map<std::string, wowobj *> *>(getItemOfCurStack(1)->value);
                    if (tmp.count(*static_cast<std::string *>(getItemOfCurStack()->value))) {
                        curStack.pop_back();
                        curStack.push_back(tmp[*static_cast<std::string *>(getItemOfCurStack()->value)]);
                    }
                    else {
                        throw "undefined field of class";
                    }
                    break;
                }
                case Element::EVAL_METHOD:
                    break;
                case Element::GET_VALUE_INT: {
                    void *tmpInt = new int(curOp.intValue);
                    curStack.push_back(new wowobj(wowobj::INT, tmpInt));
                    break;
                }
                case Element::GET_VALUE_DOUBLE: {
                    void *tmpDouble = new double(curOp.doubleValue);
                    curStack.push_back(new wowobj(wowobj::DOUBLE, tmpDouble));
                    break;
                }
                case Element::GET_VALUE_STR: {
                    void *tmpString = new std::string(curOp.stringValue);
                    curStack.push_back(new wowobj(wowobj::STRING, tmpString));
                    break;
                }
                case Element::GET_VALUE_NONE: {
                    curStack.push_back(new wowobj(wowobj::NONE));
                    break;
                }
                case Element::GET_VALUE_BOOL: {
                    void *tmpBool = new bool(curOp.intValue);
                    curStack.push_back(new wowobj(wowobj::BOOL, tmpBool));
                    break;
                }
                case Element::CLEAR_STACK: {
                    curStack.pop_back();
                    break;
                }
                case Element::POSITIVE_JMP: {
                    if (getItemOfCurStack()->type == wowobj::BOOL) {
                        if (*(static_cast<bool *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            continue;
                        }
                    }
                    else if (getItemOfCurStack()->type == wowobj::INT) {
                        if (*(static_cast<int *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            continue;
                        }
                    }
                    else {
                        throw "invalid type for comp";
                    }
                    break;
                }
                case Element::NEGATIVE_JMP: {
                    if (getItemOfCurStack()->type == wowobj::BOOL) {
                        if (!*(static_cast<bool *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            continue;
                        }
                    }
                    else if (getItemOfCurStack()->type == wowobj::INT) {
                        if (!*(static_cast<int *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            continue;
                        }
                    }
                    else {
                        throw "invalid type for comp";
                    }
                    break;
                }
                case Element::JMP: {
                    i = curOp.posJump - 1;
                    break;
                }
                case Element::CMP_EQUAL: {
                    if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                        && getItemOfCurStack(1)->type == wowobj::INT) {
                        bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value)
                                   == *static_cast<int *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::DOUBLE) {
                        bool tmp = *static_cast<double *>(getItemOfCurStack(1)->value)
                                   == *static_cast<double *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::BOOL) {
                        bool tmp = *static_cast<bool *>(getItemOfCurStack(1)->value)
                                   == *static_cast<bool *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::STRING) {
                        bool tmp = *static_cast<std::string *>(getItemOfCurStack(1)->value)
                                   == *static_cast<std::string *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else {
                        throw "no compare vars";
                    }
                    break;
                }
                case Element::CMP_NOT_EQUAL: {
                    if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                        && getItemOfCurStack(1)->type == wowobj::INT) {
                        bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value)
                                   != *static_cast<int *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::DOUBLE) {
                        bool tmp = *static_cast<double *>(getItemOfCurStack(1)->value)
                                   != *static_cast<double *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::BOOL) {
                        bool tmp = *static_cast<bool *>(getItemOfCurStack(1)->value)
                                   != *static_cast<bool *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::STRING) {
                        bool tmp = *static_cast<std::string *>(getItemOfCurStack(1)->value)
                                   != *static_cast<std::string *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else {
                        throw "no compare vars";
                    }
                    break;
                }
                case Element::CMP_LESS: {
                    if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                        && getItemOfCurStack(1)->type == wowobj::INT) {
                        bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value)
                                   < *static_cast<int *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::DOUBLE) {
                        bool tmp = *static_cast<double *>(getItemOfCurStack(1)->value)
                                   < *static_cast<double *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::STRING) {
                        bool tmp = *static_cast<std::string *>(getItemOfCurStack(1)->value)
                                   < *static_cast<std::string *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else {
                        throw "no compare vars";
                    }
                    break;
                }
                case Element::CMP_MORE: {
                    if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                        && getItemOfCurStack(1)->type == wowobj::INT) {
                        bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value)
                                   > *static_cast<int *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::DOUBLE) {
                        bool tmp = *static_cast<double *>(getItemOfCurStack(1)->value)
                                   > *static_cast<double *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::STRING) {
                        bool tmp = *static_cast<std::string *>(getItemOfCurStack(1)->value)
                                   > *static_cast<std::string *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else {
                        throw "no compare vars";
                    }
                    break;
                }
                case Element::CMP_MORE_EQUAL: {
                    if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                        && getItemOfCurStack(1)->type == wowobj::INT) {
                        bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value)
                                   >= *static_cast<int *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::DOUBLE) {
                        bool tmp = *static_cast<double *>(getItemOfCurStack(1)->value)
                                   >= *static_cast<double *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::STRING) {
                        bool tmp = *static_cast<std::string *>(getItemOfCurStack(1)->value)
                                   >= *static_cast<std::string *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else {
                        throw "no compare vars";
                    }
                    break;
                }
                case Element::CMP_LESS_EQUAL: {
                    if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                        && getItemOfCurStack(1)->type == wowobj::INT) {
                        bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value)
                                   <= *static_cast<int *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::DOUBLE) {
                        bool tmp = *static_cast<double *>(getItemOfCurStack(1)->value)
                                   <= *static_cast<double *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::STRING) {
                        bool tmp = *static_cast<std::string *>(getItemOfCurStack(1)->value)
                                   <= *static_cast<std::string *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else {
                        throw "no compare vars";
                    }
                    break;
                }
                case Element::BEGIN_SCOPE: {
                    scopes.emplace_back();
                    break;
                }
                case Element::END_SCOPE: {
                    for (const auto &now : scopes.back().vars) {
                        bigScopes.back().vars.erase(now.first);
                    }
                    for (const auto &now : scopes.back().funcs) {
                        bigScopes.back().funcs.erase(now.first);
                    }
                    for (const auto &now : scopes.back().classes) {
                        bigScopes.back().classes.erase(now.first);
                    }
                    scopes.pop_back();
                }
                case Element::RETURN_VALUE: {
                    curStack.push_back({getItemOfCurStack()});
                    i = stackTrace.back().poliz->operations.size();
                    break;
                }
                case Element::DEL:
                    break;
                case Element::PLUS_INPLACE:
                    break;
                case Element::MINUS_INPLACE:
                    break;
                case Element::POW_INPLACE:
                    break;
                case Element::MUL_INPLACE:
                    break;
                case Element::DIV_INPLACE:
                    break;
                case Element::INTDIV_INPLACE:
                    break;
                case Element::MOD_INPLACE:
                    break;
                case Element::OR_BIT_INPLACE:
                    break;
                case Element::AND_BIT_INPLACE:
                    break;
                case Element::XOR_INPLACE:
                    break;
                case Element::LEFT_SHIFT_INPLACE:
                    break;
                case Element::RIGHT_SHIFT_INPLACE:
                    break;
            }
        }
        if (i == stackTrace.back().poliz->operations.size() + 1) {
            curStack.push_back(new wowobj(wowobj::NONE));
        }

        for (const auto &now : scopes.back().vars) {
            bigScopes.back().vars.erase(now.first);
        }
        for (const auto &now : scopes.back().funcs) {
            bigScopes.back().funcs.erase(now.first);
        }
        for (const auto &now : scopes.back().classes) {
            bigScopes.back().classes.erase(now.first);
        }
        auto returnValue = scopes.back().vars;
        scopes.pop_back();
        stackTrace.pop_back();
        return returnValue;
    }

public:
    int run(Poliz *wowByteCode, std::string name = "") {
        stackTrace.push_back({name, wowByteCode});
        bigScopes.emplace_back();
        scopes.emplace_back();
        runLevel();
        return 0;
    }

};

#endif //WOW_VM_H