#include "tokenizer_tests/tokenizer_tests.h"
#include <iostream>

int main() {
    std::cout << "Start tests.\n";

    std::cout << "Running tokenizer_tests:\n";
    TokenizerTests tokenTests;
    tokenTests.run();

    std::cout << "Ended tokenizer_tests\n";
    if (tokenTests.getCountFailedTestsP() != 0) {
        std::cout << "\x1b[31m";
    }
    std::cout << "Positive:\n";
    std::cout << tokenTests.getCountTestsP() - tokenTests.getCountFailedTestsP() << "/"
              << tokenTests.getCountTestsP()
              << "\n";
    if (tokenTests.getCountFailedTestsP() != 0) {
        std::cout << "\x1b[0m";
    }

    if (tokenTests.getCountFailedTestsN() != 0) {
        std::cout << "\x1b[31m";
    }
    std::cout << "Negative:\n";
    std::cout << tokenTests.getCountTestsN() - tokenTests.getCountFailedTestsN() << "/" << tokenTests.getCountTestsN()
              << "\n";
    if (tokenTests.getCountFailedTestsN() != 0) {
        std::cout << "\x1b[0m";
    }

    if (tokenTests.getCountFailedTests() != 0) {
        std::cout << "\x1b[31m";
    } else {
        std::cout << "\x1b[32m";
    }
    std::cout << "All:\n";
    std::cout << tokenTests.getCountTests() - tokenTests.getCountFailedTests() << "/" << tokenTests.getCountTests()
              << "\n";
    std::cout << "\x1b[0m";
    std::cout << "Ended tests";
    return 0;
}