#ifndef WOW_FREAD_H
#define WOW_FREAD_H

#include <sstream>
#include <fstream>
#include <string>

std::string readFile(const std::string fileName) {
    std::ifstream f(fileName);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

#endif //WOW_FREAD_H
