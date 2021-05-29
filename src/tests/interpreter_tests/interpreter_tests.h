#ifndef WOW_INTERPRETER_TESTS_H
#define WOW_INTERPRETER_TESTS_H

#include "../../interpreter/interpreter.h"
#include "../../utils/fread.h"
#include <iostream>
#include <string>


class InterpreterTests {
public:
    void run() {
        run_tests();
    }

    InterpreterTests() {
        loadCountTests();
    }

private:
    int COUNT_TESTS_P{};
    int COUNT_TESTS_N{};

    int COUNT_FAILED_TESTS_P = 0;
    int COUNT_FAILED_TESTS_N = 0;

    const std::string prefixPath = "tests_files/interpreter_tests/";

    void loadCountTests() {
        COUNT_TESTS_P = stoi(readFile(prefixPath + "positives/countTests"));
        COUNT_TESTS_N = stoi(readFile(prefixPath + "negatives/countTests"));
    }

public:
    [[nodiscard]] int getCountTestsP() const {
        return COUNT_TESTS_P;
    }

    [[nodiscard]] int getCountTestsN() const {
        return COUNT_TESTS_N;
    }

    [[nodiscard]] int getCountTests() const {
        return COUNT_TESTS_P + COUNT_TESTS_N;
    }

    [[nodiscard]] int getCountFailedTestsP() const {
        return COUNT_FAILED_TESTS_P;
    }

    [[nodiscard]] int getCountFailedTestsN() const {
        return COUNT_FAILED_TESTS_N;
    }

    [[nodiscard]] int getCountFailedTests() const {
        return COUNT_FAILED_TESTS_P + COUNT_FAILED_TESTS_N;
    }

private:
    void run_tests() {

        COUNT_FAILED_TESTS_P = 0;
        for (int i = 1; i <= COUNT_TESTS_P; i++) {
            std::string code = readFile(prefixPath + "positives/test_" + std::to_string(i) + ".code");
            std::string input = readFile(prefixPath + "positives/test_" + std::to_string(i) + ".input");
            std::string output = readFile(prefixPath + "positives/test_" + std::to_string(i) + ".output");
            auto scriptInput = std::stringstream(input);
            auto scriptOutput = std::stringstream();
            auto scriptErrors = std::stringstream();
            Runner runner(code, &scriptInput, &scriptOutput, &scriptOutput);
            if (scriptOutput.str() == output && scriptErrors.str().empty()) {
                std::cout << "Test #" << i << ": OK\n";
            }
            else {
                std::cout << "Test #" << i << ": Failed\n";
                COUNT_FAILED_TESTS_P++;
            }
        }
        COUNT_FAILED_TESTS_N = 0;
        for (int i = 1; i <= COUNT_TESTS_N; i++) {
            std::string code = readFile(prefixPath + "negatives/test_" + std::to_string(i) + ".code");
            std::string input = readFile(prefixPath + "negatives/test_" + std::to_string(i) + ".input");
            std::string output = readFile(prefixPath + "negatives/test_" + std::to_string(i) + ".output");
            std::string errors = readFile(prefixPath + "negatives/test_" + std::to_string(i) + ".errors");
            auto scriptInput = std::stringstream(input);
            auto scriptOutput = std::stringstream();
            auto scriptErrors = std::stringstream();
            Runner runner(code, &scriptInput, &scriptOutput, &scriptOutput);
            if (scriptOutput.str() == output && scriptErrors.str() == errors) {
                std::cout << "Test #" << i << ": OK\n";
            }
            else {
                std::cout << "Test #" << i << ": Failed\n";
                COUNT_FAILED_TESTS_N++;
            }
        }
    }
};

#endif //WOW_INTERPRETER_TESTS_H
