#include "tokenizer_tests/tokenizer_tests.h"
#include "syntaxCheck_tests/syntaxCheck_tests.h"
#include <iostream>

void tokenizeTests(){
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
}

void syntaxCheckTests(){
    std::cout << "Running syntaxCheck tests:\n";
    SyntaxCheckTests syntaxCheckTests;
    syntaxCheckTests.run();

    std::cout << "Ended syntaxCheck tests\n";
    if (syntaxCheckTests.getCountFailedTestsP() != 0) {
        std::cout << "\x1b[31m";
    }
    std::cout << "Positive:\n";
    std::cout << syntaxCheckTests.getCountTestsP() - syntaxCheckTests.getCountFailedTestsP() << "/"
              << syntaxCheckTests.getCountTestsP()
              << "\n";
    if (syntaxCheckTests.getCountFailedTestsP() != 0) {
        std::cout << "\x1b[0m";
    }

    if (syntaxCheckTests.getCountFailedTestsN() != 0) {
        std::cout << "\x1b[31m";
    }
    std::cout << "Negative:\n";
    std::cout << syntaxCheckTests.getCountTestsN() - syntaxCheckTests.getCountFailedTestsN() << "/" << syntaxCheckTests.getCountTestsN()
              << "\n";
    if (syntaxCheckTests.getCountFailedTestsN() != 0) {
        std::cout << "\x1b[0m";
    }

    if (syntaxCheckTests.getCountFailedTests() != 0) {
        std::cout << "\x1b[31m";
    } else {
        std::cout << "\x1b[32m";
    }
    std::cout << "All:\n";
    std::cout << syntaxCheckTests.getCountTests() - syntaxCheckTests.getCountFailedTests() << "/" << syntaxCheckTests.getCountTests()
              << "\n";
    std::cout << "\x1b[0m";
}

int main() {
    std::cout << "Start tests.\n";
    tokenizeTests();
    syntaxCheckTests();
    std::cout << "Ended tests";
    return 0;
}