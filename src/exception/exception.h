#ifndef WOW_EXCEPTION_H
#define WOW_EXCEPTION_H

#include <string>

class Exception {
public:
    std::string message;
    int line, posInLine;

    Exception(std::string message) :
            message(message) {}
};

#endif //WOW_EXCEPTION_H
