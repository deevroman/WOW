#ifndef WOW_EXCEPTION_H
#define WOW_EXCEPTION_H

#include <string>

class Exception {
public:
    std::string message;
    int line = -1, posInLine = -1;

    Exception(std::string message) :
            message(message) {};
    Exception(std::string Message, int Line, int Pos):
        message(Message), line(Line), posInLine(Pos){};
};

#endif //WOW_EXCEPTION_H
