#include <iostream>
#include "tokenizer_tests/tokenizer_tests.h"
#include "interpreter_tests/interpreter_tests.h"


void tokenizeTests() {
    std::cout << "Running tokenizer_tests:\n";
    TokenizerTests tokenTests;
    tokenTests.run();

    std::cout << "Ended tokenizer_tests\n";
    if (tokenTests.getCountFailedTestsP() != 0) {
        std::cout << "\x1b[31m";
    }
    std::cout << "Positive:\n"
              << tokenTests.getCountTestsP() - tokenTests.getCountFailedTestsP() << "/"
              << tokenTests.getCountTestsP()
              << "\n";
    if (tokenTests.getCountFailedTestsP() != 0) {
        std::cout << "\x1b[0m";
    }

    if (tokenTests.getCountFailedTestsN() != 0) {
        std::cout << "\x1b[31m";
    }
    std::cout << "Negative:\n"
              << tokenTests.getCountTestsN() - tokenTests.getCountFailedTestsN() << "/" << tokenTests.getCountTestsN()
              << "\n";
    if (tokenTests.getCountFailedTestsN() != 0) {
        std::cout << "\x1b[0m";
    }

    if (tokenTests.getCountFailedTests() != 0) {
        std::cout << "\x1b[31m";
    }
    else {
        std::cout << "\x1b[32m";
    }
    std::cout << "All:\n"
              << tokenTests.getCountTests() - tokenTests.getCountFailedTests() << "/" << tokenTests.getCountTests()
              << "\n"
              << "\x1b[0m";
}


//void interpreterTests(){
//    std::cout << "Running translator tests:\n";
//    InterpreterTests interpreterTests;
//    interpreterTests.run();
//
//    std::cout << "Ended translator tests\n";
//    if (interpreterTests.getCountFailedTestsP() != 0) {
//        std::cout << "\x1b[31m";
//    }
//    std::cout << "Positive:\n";
//    std::cout << interpreterTests.getCountTestsP() - interpreterTests.getCountFailedTestsP() << "/"
//              << interpreterTests.getCountTestsP()
//              << "\n";
//    if (interpreterTests.getCountFailedTestsP() != 0) {
//        std::cout << "\x1b[0m";
//    }
//
//    if (interpreterTests.getCountFailedTestsN() != 0) {
//        std::cout << "\x1b[31m";
//    }
//    std::cout << "Negative:\n";
//    std::cout << interpreterTests.getCountTestsN() - interpreterTests.getCountFailedTestsN() << "/" << interpreterTests.getCountTestsN()
//              << "\n";
//    if (interpreterTests.getCountFailedTestsN() != 0) {
//        std::cout << "\x1b[0m";
//    }
//
//    if (interpreterTests.getCountFailedTests() != 0) {
//        std::cout << "\x1b[31m";
//    } else {
//        std::cout << "\x1b[32m";
//    }
//    std::cout << "All:\n";
//    std::cout << interpreterTests.getCountTests() - interpreterTests.getCountFailedTests() << "/" << interpreterTests.getCountTests()
//              << "\n";
//    std::cout << "\x1b[0m";
//}

int main() {
    std::cout << "Start tests.\n";
    tokenizeTests();
//    interpreterTests();
    std::cout << "Ended tests";
    return 0;
}