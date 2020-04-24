#ifndef WOW_VM_H
#define WOW_VM_H

#include <string>
#include <vector>
#include <map>
#include "../utils/poliz.h"
#include "../utils/wowobj.h"

class VM {
private:
    class curLevel {
    public:
        std::string name;
        Poliz *poliz;
        std::vector<wowobj *> curStack;
        std::map<std::string, Poliz *> scope;
        std::map<std::string, wowobj *> TID;
    };

    std::vector<curLevel> stackTrace;

    void runLevel() {
        auto &curTrace = stackTrace.back();
        for (const auto &curOp : curTrace.poliz->operations) {
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
                case Element::PLUS:
                    break;
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
                case Element::INDEX_VALUE:
                    break;
                case Element::DEF_FUNC:
                    break;
                case Element::DEF_CLASS:
                    break;
                case Element::CALL_FUNC:
                    break;
                case Element::CREATE_CLASS:
                    break;
                case Element::GET_VALUE:
                    break;
                case Element::GET_FIELD:
                    break;
                case Element::EVAL_METHOD:
                    break;
                case Element::GET_VALUE_INT:
                    void *tmpInt = new int(curOp.intValue);
                    curTrace.curStack.push_back(new wowobj(wowobj::INT, tmpInt));
                    break;
                case Element::GET_VALUE_DOUBLE:
                    void *tmpDouble = new double(curOp.doubleValue);
                    curTrace.curStack.push_back(new wowobj(wowobj::DOUBLE, tmpDouble));
                    break;
                case Element::GET_VALUE_STR:
                    void *tmpString = new std::string(curOp.stringValue);
                    curTrace.curStack.push_back(new wowobj(wowobj::STRING, tmpString));
                    break;
                case Element::GET_VALUE_NONE:
                    curTrace.curStack.push_back(new wowobj(wowobj::NONE));
                    break;
                case Element::GET_VALUE_BOOL:
                    void *tmpBool = new bool(curOp.intValue);
                    curTrace.curStack.push_back(new wowobj(wowobj::BOOL, tmpBool));
                    break;

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
