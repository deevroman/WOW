
#ifndef WOW_VM_H
#define WOW_VM_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include "../utils/poliz.h"
#include "../utils/wowobj.h"
#include "../utils/exception.h"

#define COMPARE(OPERATION) \
if (getItemOfCurStack(1)->type == getItemOfCurStack()->type && getItemOfCurStack(1)->type == wowobj::INT) { \
    bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value) \
               OPERATION *static_cast<int *>(getItemOfCurStack()->value); \
    curStack.pop_back(); \
    curStack.pop_back(); \
    curStack.push_back(new wowobj(wowobj::BOOL, static_cast<void*>(new bool(tmp)))); \
} \
else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type && getItemOfCurStack(1)->type == wowobj::DOUBLE) { \
    bool tmp = *static_cast<double *>(getItemOfCurStack(1)->value) \
               OPERATION *static_cast<double *>(getItemOfCurStack()->value); \
    curStack.pop_back(); \
    curStack.pop_back(); \
    curStack.push_back(new wowobj(wowobj::BOOL, static_cast<void*>(new bool(tmp)))); \
} \
else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type && getItemOfCurStack(1)->type == wowobj::STRING) { \
    bool tmp = *static_cast<std::string *>(getItemOfCurStack(1)->value) \
               OPERATION *static_cast<std::string *>(getItemOfCurStack()->value); \
    curStack.pop_back(); \
    curStack.pop_back(); \
    curStack.push_back(new wowobj(wowobj::BOOL, static_cast<void*>(new bool(tmp)))); \
} \
else { \
    throw Exception("no compare vars", curOp.numLine, curOp.numPos); \
}


class VM {
private:
    struct scope {
        std::map<std::string, std::pair<Poliz *, bool>> funcs;
        std::map<std::string, std::pair<Poliz *, bool>> classes;
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
    std::istream *inputStream{};
    std::ostream *outputStream{};

    wowobj *getItemOfCurStack(int number = 0) {
        return curStack[curStack.size() - 1 - number];
    }

    std::map<std::string, wowobj *> runLevel() {
        auto &curLevel = stackTrace.back();
        int i = 0;
        for (; i < stackTrace.back().poliz->operations.size(); i++) {
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

                    else if (getItemOfCurStack(1)->type == wowobj::INT
                             && getItemOfCurStack(0)->type == wowobj::INT) {
                        bool value = *(static_cast<int *>(getItemOfCurStack(1)->value)) or
                                     *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                    else if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        bool value = *(static_cast<int *>(getItemOfCurStack(1)->value)) and
                                     *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                    else if (getItemOfCurStack(1)->type == wowobj::DOUBLE
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
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
                    else if (getItemOfCurStack()->type == wowobj::LIST) {
                        getItemOfCurStack(1)->type = wowobj::LIST;
                        getItemOfCurStack(1)->value = new std::vector<wowobj *>(
                                *static_cast<std::vector<wowobj *> *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                    }
                    else {
                        // todo
                    }
                    break;
                }
                case Element::INIT_PARAM: {
                    bigScopes.back().vars[curOp.stringValue]
                            = scopes.back().vars[curOp.stringValue]
                            = getItemOfCurStack();
                    curStack.pop_back();
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
                            if (ind < static_cast<std::vector<wowobj *> *>(getItemOfCurStack(1)->value)->size()) {
                                auto tmp = (*static_cast<std::vector<wowobj *> *>(getItemOfCurStack(1)->value))[ind];
                                curStack.pop_back();
                                curStack.pop_back();
                                curStack.push_back(tmp);
                            }
                            else {
                                throw Exception("index error", curOp.numLine, curOp.numPos);
                            }
                        }
                    }
                    break;
                }
                case Element::DEF_FUNC: {
                    bigScopes.back().funcs[curOp.stringValue].first = scopes.back().funcs[curOp.stringValue].first = curLevel.poliz->funcs[curOp.stringValue];
                    bigScopes.back().funcs[curOp.stringValue].second = scopes.back().funcs[curOp.stringValue].second = true;
                    break;
                }
                case Element::DEF_CLASS: {
                    bigScopes.back().classes[curOp.stringValue].first = scopes.back().classes[curOp.stringValue].first = curLevel.poliz->classes[curOp.stringValue];
                    bigScopes.back().classes[curOp.stringValue].second = scopes.back().classes[curOp.stringValue].second = true;
                    break;
                }
                case Element::CALL_FUNC: {
                    if (curOp.stringValue == "input") {
                        auto *tmp = new std::string;
                        *inputStream >> *tmp;
                        curStack.push_back(new wowobj(wowobj::STRING, static_cast<void *>(tmp)));
                    }
                    else if (curOp.stringValue == "print") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::STRING) {
                            *outputStream << *(static_cast<std::string *>(arg->value));
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::NONE));
                        }
                        else if (arg->type == wowobj::INT) {
                            *outputStream << *(static_cast<int *>(arg->value));
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::NONE));
                        }
                        else if (arg->type == wowobj::DOUBLE) {
                            *outputStream << *(static_cast<double *>(arg->value));
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::NONE));
                        }
                        else if (arg->type == wowobj::BOOL) {
                            *outputStream << (*(static_cast<bool *>(arg->value)) ? "True" : "False");
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::NONE));
                        }
                        else if (arg->type == wowobj::NONE) {
                            *outputStream << "None";
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::NONE));
                        }
                        else {
                            throw Exception("no printble type", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "int") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::STRING) {
                            int *tmp = new int(std::stoi(*(static_cast<std::string *>(arg->value))));
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::INT, static_cast<void *>(tmp)));
                        }
                        else if (arg->type == wowobj::DOUBLE) {
                            int *tmp = new int((int) *(static_cast<double *>(arg->value)));
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::INT, static_cast<void *>(tmp)));
                        }
                        else {
                            throw Exception("not numbery type", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "bool") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::INT) {
                            bool *tmp = new bool((*(static_cast<int *>(arg->value))) != 0);
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::BOOL, static_cast<void *>(tmp)));
                        }
                        else if (arg->type == wowobj::BOOL) {

                        }
                        else {
                            throw Exception("not booly type", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "str") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::INT) {
                            auto *tmp = new std::string(std::to_string(*(static_cast<int *>(arg->value))));
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::STRING, static_cast<void *>(tmp)));
                        }
                        else if (arg->type == wowobj::BOOL) {
                            auto *tmp = new std::string(*(static_cast<bool *>(arg->value)) ? "True" : "False");
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::STRING, static_cast<void *>(tmp)));
                        }
                        else {
                            throw Exception("not strinble type", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "list") {
                        curStack.push_back(
                                new wowobj(wowobj::LIST, static_cast<void *>(new std::vector<wowobj *>)));
                    }
                    else if (curOp.stringValue == "len") {
                        if (getItemOfCurStack()->type == wowobj::LIST) {
                            int sz = (*(static_cast<std::vector<wowobj *> *>(getItemOfCurStack()->value))).size();
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::INT, new int(sz)));
                        }
                        else if (getItemOfCurStack()->type == wowobj::STRING) {
                            int sz = (*(static_cast<std::string *>(getItemOfCurStack()->value))).size();
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::INT, new int(sz)));
                        }
                        else {
                            throw Exception("not countable var", curOp.numLine, curOp.numPos);
                        }
                    }
                    else {
                        if (bigScopes.back().funcs.count(curOp.stringValue)) {
                            stackTrace.push_back({curOp.stringValue, bigScopes.back().funcs[curOp.stringValue].first});
                            bigScopes.push_back(scopes.back());
                            bigScopes.back().type = scope::FUNC;
                            scopes.push_back({scopes.back().funcs, {}, {}, scope::FUNC}); // todo
                            for (auto &now : scopes.back().funcs) {
                                now.second.second = false;
                            }
                            runLevel();
                        }
                        else {
                            throw Exception("Called undefined function", curOp.numLine, curOp.numPos);
                        }
                    }
                    break;
                }
                case Element::CREATE_CLASS: {
                    bigScopes.push_back({scopes.back().funcs, {}, {}, scope::CLASS});
                    scopes.push_back({{}, {}, {}, scope::CLASS});
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
                        throw Exception("undefined field of class", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::EVAL_METHOD: {
                    if (curOp.stringValue == "append") {
                        if (getItemOfCurStack(1)->type == wowobj::LIST) {
                            (*static_cast<std::vector<wowobj *> *>(getItemOfCurStack(1)->value)).push_back(
                                    getItemOfCurStack(0));
                            curStack.pop_back();
                        }
                        else {
                            throw Exception("invalid method", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "pop") {
                        if (getItemOfCurStack()->type == wowobj::LIST) {
                            if (static_cast<std::vector<wowobj *> *>(getItemOfCurStack()->value)->empty()) {
                                throw Exception("pop from empty list", curOp.numLine, curOp.numPos);
                            }
                            static_cast<std::vector<wowobj *> *>(getItemOfCurStack()->value)->pop_back();
                            curStack.pop_back();
                        }
                        else {
                            throw Exception("invalid method", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "split") {
                        if (getItemOfCurStack()->type == wowobj::STRING) {
                            auto tmp = split(*static_cast<std::string *>(getItemOfCurStack()->value), ' ');
                            auto tmp2 = new std::vector<wowobj *>;
                            for (auto &now : tmp) {
                                tmp2->push_back(new wowobj(wowobj::STRING, static_cast<void *> (new std::string(now))));
                            }
                            curStack.pop_back();
                            curStack.push_back(new wowobj(wowobj::LIST, static_cast<void*>(tmp2)));
                        }
                        else {
                            throw Exception("invalid method", curOp.numLine, curOp.numPos);
                        }
                    }
                    else {
                        throw Exception("invalid method", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
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
                        curStack.pop_back();
                    }
                    else if (getItemOfCurStack()->type == wowobj::INT) {
                        if (*(static_cast<int *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            continue;
                        }
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for comp", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::NEGATIVE_JMP: {
                    if (getItemOfCurStack()->type == wowobj::BOOL) {
                        if (!*(static_cast<bool *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            continue;
                        }
                        curStack.pop_back();
                    }
                    else if (getItemOfCurStack()->type == wowobj::INT) {
                        if (!*(static_cast<int *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            continue;
                        }
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for comp", curOp.numLine, curOp.numPos);
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
                        throw Exception("no compare vars", curOp.numLine, curOp.numPos);
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
                        throw Exception("no compare vars", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::CMP_LESS: {
                    COMPARE(<);
                    break;
                }
                case Element::CMP_MORE: {
                    COMPARE(>)
                    break;
                }
                case Element::CMP_MORE_EQUAL: {
                    COMPARE(>=)
                    break;
                }
                case Element::CMP_LESS_EQUAL: {
                    COMPARE(<=)
                    break;
                }
                case Element::BEGIN_SCOPE: {
                    scopes.push_back({scopes.back().funcs});
                    for (auto &now : scopes.back().funcs) {
                        now.second.second = false;
                    }
                    break;
                }
                case Element::END_SCOPE: {
                    for (const auto &now : scopes.back().vars) {
                        bigScopes.back().vars.erase(now.first);
                    }
                    for (const auto &now : scopes.back().funcs) {
                        if (now.second.second == 1) {
                            bigScopes.back().funcs.erase(now.first);
                        }
                    }
                    for (const auto &now : scopes.back().classes) {
                        if (now.second.second == 1) {
                            bigScopes.back().classes.erase(now.first);
                        }
                    }
                    scopes.pop_back();
                }
                case Element::RETURN_VALUE: {
//                    curStack.push_back({getItemOfCurStack()});
                    i = stackTrace.back().poliz->operations.size() + 1; // чтобы не совпало с jmp
                    break;
                }
                case Element::DEL: {
                    //todo
                    curStack.pop_back();
                    break;
                }
                case Element::XOR_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) ^
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::OR_BIT_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) |
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::AND_BIT_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) &
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::LEFT_SHIFT_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) <<
                                                                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::RIGHT_SHIFT_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) >>
                                                                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::PLUS_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) +
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else if (getItemOfCurStack(1)->type == wowobj::DOUBLE
                             && getItemOfCurStack(0)->type == wowobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) +
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<double *>(curStack.back()->value) = value;
                    }
                    else if (getItemOfCurStack(1)->type == wowobj::STRING
                             && getItemOfCurStack()->type == wowobj::STRING) {
                        std::string value = (*(static_cast<std::string *>(getItemOfCurStack(1)->value)))
                                            + (*(static_cast<std::string *>(getItemOfCurStack()->value)));
                        curStack.pop_back();
                        *static_cast<std::string *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MINUS_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) -
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else if (getItemOfCurStack(1)->type == wowobj::DOUBLE
                             && getItemOfCurStack(0)->type == wowobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) -
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<double *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MUL_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) *
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else if (getItemOfCurStack(1)->type == wowobj::DOUBLE
                             && getItemOfCurStack(0)->type == wowobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) *
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<double *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                }
                case Element::DIV_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        double value = (double) *(static_cast<int *>(getItemOfCurStack(1)->value)) /
                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        curStack.back()->type = wowobj::DOUBLE;
                        *static_cast<double *>(curStack.back()->value) = value;
                    }
                    else if (getItemOfCurStack(1)->type == wowobj::DOUBLE
                             && getItemOfCurStack(0)->type == wowobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) /
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        curStack.back()->type = wowobj::DOUBLE;
                        *static_cast<double *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MOD_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) %
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::INTDIV_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) /
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::POW_INPLACE: {
                    if (getItemOfCurStack(1)->type == wowobj::INT
                        && getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = pow(*(static_cast<int *>(getItemOfCurStack(1)->value)),
                                        *(static_cast<int *>(getItemOfCurStack()->value)));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MAKE_LIST: {
                    auto tmp = new wowobj(wowobj::LIST,
                                          static_cast<void *>(new std::vector<wowobj *>));
                    static_cast<std::vector<wowobj *> *>(tmp->value)->reserve(curOp.countParams);
                    for (int j = 0; j < curOp.countParams; j++) {
                        static_cast<std::vector<wowobj *> *>(tmp->value)->push_back(getItemOfCurStack());
                        curStack.pop_back();
                    }
                    std::reverse(static_cast<std::vector<wowobj *> *>(tmp->value)->begin(),
                                 static_cast<std::vector<wowobj *> *>(tmp->value)->end());
                    curStack.push_back(tmp);
                    break;
                }
            }
        }
        if (i != stackTrace.back().poliz->operations.size() + 1 + 1) {
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
        bigScopes.pop_back();
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

    VM() {}

    VM(std::istream *inputStream, std::ostream *outputStream) : inputStream(inputStream), outputStream(
            outputStream) {}
};

#endif //WOW_VM_H