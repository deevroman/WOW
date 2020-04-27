#ifndef WOW_VM_H
#define WOW_VM_H

#include <string>
#include <vector>
#include <map>
#include "../utils/poliz.h"
#include "../utils/wowobj.h"

class VM {
private:
    class Level {
    public:
        std::string name;
        Poliz *poliz;
        std::vector<wowobj *> curStack;
        std::map<std::string, Poliz *> scope;
        std::map<std::string, wowobj *> *TID = nullptr;
    };

    std::vector<Level> stackTrace;

    wowobj *getItemOfCurStack(int number = 0, int numberOfStack = 0) {
        int sz = stackTrace[stackTrace.size() - 1 - numberOfStack].curStack.size();
        return stackTrace[stackTrace.size() - 1 - numberOfStack].curStack[sz - 1 - number];
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
                        stackTrace.back().curStack.pop_back();
                        stackTrace.back().curStack.pop_back();
                        stackTrace.back().curStack.push_back(tmpobj);
                    }
                    break;
                }
                case Element::MINUS:
                    break;
                case Element::MUL:
                    break;
                case Element::DIV:
                    break;
                case Element::MOD:
                    break;
                case Element::INTDIV:
                    break;
                case Element::UNAR_PLUS:
                    break;
                case Element::UNAR_MINUS:
                    break;
                case Element::UNAR_TILDA:
                    break;
                case Element::POW:
                    break;
                case Element::COPY: {
                    if (getItemOfCurStack()->type == wowobj::INT
                        || getItemOfCurStack()->type == wowobj::DOUBLE
                        || getItemOfCurStack()->type == wowobj::STRING) {
                        getItemOfCurStack(1)->value = getItemOfCurStack()->value;
                        getItemOfCurStack(1)->type = getItemOfCurStack()->type;
                    }
                    break;
                }
                case Element::GET_VALUE: {
                    if (curLevel.TID.count(curOp.stringValue) == 0) {
                        curLevel.curStack.push_back(curLevel.TID[curOp.stringValue]);
                    }
                    else {
                        curLevel.curStack.push_back(new wowobj(wowobj::NONE));
                        curLevel.TID[curOp.stringValue] = curLevel.curStack.back();
                    }
                    break;
                }
                case Element::INDEX_VALUE:
                    break;
                case Element::DEF_FUNC:
                    break;
                case Element::DEF_CLASS:
                    break;
                case Element::CALL_FUNC: {
                    if (curOp.stringValue == "input") {
                        std::string tmp; // TODO наверное можно нормально
                        std::cin >> tmp;
                        auto tmp2 = new std::string(tmp);
                        curLevel.curStack.push_back(new wowobj(wowobj::STRING), static_cast<void *>(tmp2));
                    }
                    else if (curOp.stringValue == "print") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == wowobj::STRING) {
                            std::cout << *(static_cast<std::string *>arg->value);
                        }
                        else if (arg->type == wowobj::INT) {
                            std::cout << *(static_cast<int *>arg->value);
                        } // TODO
                    }
                    else if (curOp.stringValue == "int") {
                        auto arg = getItemOfCurStack();
                        
                        if (arg->type == wowobj::STRING) {
                            curLevel.curStack.push_back(new wowobj(wowobj::STRING), static_cast<void *>(tmp2));
                        } else {
                            throw "TODO"; // TODO
                        }
                    }
                    break;
                }
                case Element::CREATE_CLASS:
                    break;
                case Element::GET_FIELD:
                    break;
                case Element::EVAL_METHOD:
                    break;
                case Element::GET_VALUE_INT: {
                    void *tmpInt = new int(curOp.intValue);
                    curLevel.curStack.push_back(new wowobj(wowobj::INT, tmpInt));
                    break;
                }
                case Element::GET_VALUE_DOUBLE: {
                    void *tmpDouble = new double(curOp.doubleValue);
                    curLevel.curStack.push_back(new wowobj(wowobj::DOUBLE, tmpDouble));
                    break;
                }
                case Element::GET_VALUE_STR: {
                    void *tmpString = new std::string(curOp.stringValue);
                    curLevel.curStack.push_back(new wowobj(wowobj::STRING, tmpString));
                    break;
                }
                case Element::GET_VALUE_NONE: {
                    curLevel.curStack.push_back(new wowobj(wowobj::NONE));
                    break;
                }
                case Element::GET_VALUE_BOOL: {
                    void *tmpBool = new bool(curOp.intValue);
                    curLevel.curStack.push_back(new wowobj(wowobj::BOOL, tmpBool));
                    break;
                }
            }
        }
    }

public:
    int run(Poliz *wowByteCode, std::string name = "") {
        stackTrace.push_back({name, wowByteCode});
        runLevel();
        return 0;
    }

};

#endif //WOW_VM_H
