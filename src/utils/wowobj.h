#ifndef WOW_WOWOBJ_H
#define WOW_WOWOBJ_H

class wowobj {
public:
    enum {
        INT, DOUBLE, BOOL, STRING, LIST, DICT, USER_CLASS, NONE
    } type;
    void *value;
    wowobj(){}
    wowobj(decltype(type) type, void* value = nullptr) : type(type), value(value){};
    class User_Class {
    public:
        Poliz* poliz;
        std::map<std::string, wowobj*> fields;
    };
};

#endif //WOW_WOWOBJ_H
