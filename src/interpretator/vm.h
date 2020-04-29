
#ifndef WOW_VM_H
#define WOW_VM_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include "../utils/poliz.h"
#include "../utils/wowobj.h"

class VM {
private:
    struct scope {
        std::map<std::string, Poliz *> funcs;
        std::map<std::string, std::pair<Poliz *, wowobj *>> classes;
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

    void runLevel() {
        auto &curLevel = stackTrace.back();
        for (int i = 0; i < stackTrace.back().poliz->operations.size(); i++) {
            auto &curOp = stackTrace.back().poliz->operations[i];
            switch (curOp.TYPE) {
                case Element::OR:
                    break;
                case Element::AND:
                    break;
                case Element::NOT:
                    break;
                case Element::XOR:
                    break;
                case Element::OR_BIT:
                    break;
                case Element::AND_BIT:
                    break;
                case Element::SHIFT_LEFT_BIN:
                    break;
                case Element::SHIFT_RIGHT_BIN:
                    break;
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
                    else if (getItemOfCurStack(1)->type == wowobj::STRING
                             && getItemOfCurStack()->type == wowobj::STRING) {
                        std::string value = (*(static_cast<std::string *>(getItemOfCurStack(1)->value)))
                                            + (*(static_cast<std::string *>(getItemOfCurStack()->value)));
                        auto tmpobj = new wowobj(wowobj::STRING, new std::string(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
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
                    break;
                }
                case Element::DIV:
                    break;
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
                    break;
                }
                case Element::INTDIV:
                    break;
                case Element::UNAR_PLUS: {
                    if (getItemOfCurStack(0)->type == wowobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new wowobj(wowobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        // TODO
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
                    else {
                        // TODO
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
                        // TODO
                    }
                    break;
                }
                case Element::POW:
                    break;
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
                    break;
                }
                case Element::CALL_FUNC: {
                    if (curOp.stringValue == "input") {
                        std::string tmp; // TODO наверное можно нормально
                        std::cin >> tmp;
                        auto tmp2 = new std::string(tmp);
                        curStack.push_back(new wowobj(wowobj::STRING, static_cast<void *>(tmp2)));
                    }
                    else if (curOp.stringValue == "print") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::STRING) {
                            std::cout << *(static_cast<std::string *>(arg->value));
                        }
                        else if (arg->type == wowobj::INT) {
                            std::cout << *(static_cast<int *>(arg->value));
                        } // TODO
                    }
                    else if (curOp.stringValue == "int") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::STRING) {
                            int *tmp = new int(std::stoi(*(static_cast<std::string *>(arg->value))));
                            curStack.push_back(new wowobj(wowobj::INT, static_cast<void *>(tmp)));
                        }
                        else {
                            throw "TODO"; // TODO
                        }
                    }
                    else if (curOp.stringValue == "str") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::INT) {
                            auto *tmp = new std::string(std::to_string(*(static_cast<int *>(arg->value))));
                            curStack.push_back(new wowobj(wowobj::STRING, static_cast<void *>(tmp)));
                        }
//                        else if (arg->type == wowobj::BOOL) {
//                            auto *tmp = new std::string(std::to_string(*(static_cast<bool *>(arg->value)))); // TODO проверить работаеть ли to_string для була
//                            curStack.push_back(new wowobj(wowobj::STRING, static_cast<void *>(tmp)));
//                        }
                        else {
                            throw "TODO"; // TODO
                        }
                    }
                    else if (curOp.stringValue == "list") {
                        curStack.push_back(
                                new wowobj(wowobj::LIST, static_cast<void *>(new std::vector<wowobj *>)));
                    }
                    else {
                        if (scopes.back().funcs.count(curOp.stringValue)) {
                            stackTrace.push_back({curOp.stringValue, scopes.back().funcs[curOp.stringValue]});
                            bigScopes.push_back({});
                            scopes.push_back({});
                            // todo передать аргументы
                        }
                        else {
                            throw "Called undefined function";
                        }
                    }
                    break;
                }
                case Element::CREATE_CLASS: {

                    break;
                }
                case Element::GET_FIELD:
                    break;
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
                            i = curOp.intValue - 1;
                            continue;
                        }
                    }
                    else if (getItemOfCurStack()->type == wowobj::INT) {
                        if (*(static_cast<int *>(getItemOfCurStack()->value))) {
                            i = curOp.intValue - 1;
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
                            i = curOp.intValue - 1;
                            continue;
                        }
                    }
                    else if (getItemOfCurStack()->type == wowobj::INT) {
                        if (!*(static_cast<int *>(getItemOfCurStack()->value))) {
                            i = curOp.intValue - 1;
                            continue;
                        }
                    }
                    else {
                        throw "invalid type for comp";
                    }
                    break;
                }
                case Element::JMP: {
                    i = curOp.intValue - 1;
                    break;
                }
                case Element::CMP_EQUAL: {
                    if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                        && getItemOfCurStack(1)->type == wowobj::INT) {
                        bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value)
                                   == *static_cast<int *>(getItemOfCurStack()->value);
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == wowobj::BOOL) {
                        bool tmp = *static_cast<bool *>(getItemOfCurStack(1)->value)
                                   == *static_cast<bool *>(getItemOfCurStack()->value);
                        curStack.push_back(new wowobj(wowobj::BOOL, new bool(tmp)));
                    }
                    break;
                }
                case Element::CMP_NOT_EQUAL: {
                    break;
                }
                case Element::CMP_LESS: {
                    break;
                }
                case Element::CMP_MORE: {
                    break;
                }
                case Element::CMP_MORE_EQUAL: {
                    break;
                }
                case Element::CMP_LESS_EQUAL: {
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
                    break;
                }
            }
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
        scopes.pop_back();
        stackTrace.pop_back();
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