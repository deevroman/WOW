#ifndef WOW_EXCEPTION_H
#define WOW_EXCEPTION_H
#include <string>

class Exception {
public:
    int code;
    std::string message;

    Exception(int code, std::string message) :
            code(code), message(message) {}
};

#endif //WOW_EXCEPTION_H
