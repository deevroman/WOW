#ifndef WOW_WOW_interpreter_H
#define WOW_WOW_interpreter_H

#include "../tokenizer/tokenizer.h"
#include "../translator/translator.h"
#include "vm.h"

class Runner {
public:
    explicit Runner(std::string &code) {
        Tokenizer tokenizer(code);
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
            std::cout << "Not parsed\n";
            std::cerr << e;
            return;
        }
        try {
            VM vm;
            vm.run(wowByteCode);
        } catch (std::string e) {
            std::cout << "Runtime error\n";
            std::cerr << e;
        }
    }

};

#endif //WOW_WOW_interpreter_H
