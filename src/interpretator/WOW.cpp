#include <iostream>
#include <string>
#include "../fread/fread.h"
#include "../tokenizer/tokenizer.h"
#include "../translator/translator.h"
#include "vm.h"

int main(int argc, char *argv[]) {
    std::string filePath;
    if (argc <= 1) {
        std::cout << "Usage: ./WOW <wowFilePath>";
        return 0;
    }
    std::cerr << argv[1] << "\n";

    std::string input = readFile(std::string(argv[1]));
    Tokenizer tokenizer(input);
    std::vector<Token> tokens;
    try {
        tokens = tokenizer.tokenize();
    } catch (std::string e) {
        std::cout << "Not parsed\n";
        std::cerr << e;
        return 0;
    }
    Poliz* wowByteCode;
    try {
        Translator translator(tokens);
        wowByteCode = translator.translate();
    } catch (std::string e) {
        std::cout << "Not parsed\n";
        std::cerr << e;
        return 0;
    }
    try {VM vm;
        vm.run(wowByteCode);
    } catch (std::string e) {
        std::cout << "Runtime error\n";
        std::cerr << e;
    }
    return 0;
}