#include "tokenizer_tests/tokenizer_tests.h"
#include <iostream>

int main() {
    std::cout << "Start tests.\n";

    std::cout << "Running tokenizer_tests:\n";
    TokenizerTests tokenTests;
    tokenTests.run();

    std::cout << "Ended tokenizer_tests\n";
    std::cout << "Positive:\n";
    std::cout << tokenTests.getCountTestsP() - tokenTests.getCountFailedTestsP() << "/" << tokenTests.getCountTestsP()
              << "\n";
    std::cout << "Negative:\n";
    std::cout << tokenTests.getCountTestsN() - tokenTests.getCountFailedTestsN() << "/" << tokenTests.getCountTestsN()
              << "\n";
    std::cout << "All:\n";
    std::cout << tokenTests.getCountTestsP() - tokenTests.getCountFailedTestsP()
                 + tokenTests.getCountTestsN() - tokenTests.getCountFailedTestsN()
              << "/" << tokenTests.getCountTestsP() + tokenTests.getCountTestsN() << "\n";
    std::cout << "Ended tests";
    return 0;
}