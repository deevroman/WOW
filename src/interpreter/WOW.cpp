#include <iostream>
#include <string>
#include "interpreter.h"

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    if (argc <= 1) {
        std::cout << "Usage: ./WOW <wowFilePath>";
        return 0;
    }

    Runner(readFile(std::string(argv[1])));

    return 0;
}