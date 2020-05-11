#ifndef WOW_WOW_interpreter_H
#define WOW_WOW_interpreter_H

#include "../tokenizer/tokenizer.h"
#include "../translator/translator.h"
#include "../utils/exception.h"
#include "../utils/str_split.h"
#include "vm.h"

class Runner {
public:
    Runner(std::string code, std::istream *inputStream = &std::cin,
           std::ostream *outputStream = &std::cout) {
        Tokenizer tokenizer(code);
        auto linesOfCode = split(code);
        std::vector<Token> tokens;
        try {
            tokens = tokenizer.tokenize();
        } catch (std::string e) {
            std::cout << "Not parsed\n";
            std::cerr << e;
            return;
        }
        Poliz *wowByteCode;
        try {
            Translator translator(tokens);
            wowByteCode = translator.translate();
        } catch (std::string e) {
            std::cerr << "Not translated\n"
                      << e;
            return;
        }
        try {
            VM vm(inputStream, outputStream);
            vm.run(wowByteCode);
        } catch (Exception e) {
            std::cerr << "Runtime error\n"
                      << e.message << "\n"
                      << e.line << ": " << linesOfCode[e.line - 1] << "\n";
        }
    }

};

#endif //WOW_WOW_interpreter_H
