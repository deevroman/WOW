#include "tokenizer_tests/tokenizer_tests.h"
#include <iostream>

int main(){
    std::cout << "Start tests.\n";

    std::cout << "Running tokenizer_tests:\n";
    TokenizerTests tokenTests;
    int failed_tokenizer_tests = tokenTests->run();
    std::cout << "Ended tokenizer_tests.\n";

    std::cout << "Ended tests";
    return 0;
}