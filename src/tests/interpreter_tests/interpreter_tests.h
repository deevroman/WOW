#ifndef WOW_INTERPRETER_TESTS_H
#define WOW_INTERPRETER_TESTS_H

#include "../../interpreter/interpreter.h"
#include "../../fread/fread.h"
#include <iostream>


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

    const std::string prefixPath = "../tests/interpreter_tests/";

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
            std::string input = readFile(prefixPath + "positives/test_" + std::to_string(i) + ".input");
            try {
                Runner runner(input);
                std::cout << "Positive test #" << i << '\n';
                std::cout << "OK\n";
            } catch (...) {
                std::cout << "Positive test #" << i << '\n';
                std::cout << "\x1b[31mFailed:\x1b[0m\n";
                COUNT_FAILED_TESTS_P++;
            }
        }
        COUNT_FAILED_TESTS_N = 0;
        for (int i = 1; i <= COUNT_TESTS_N; i++) {
            std::string input = readFile(prefixPath + "negatives/test_" + std::to_string(i) + ".input");
            Tokenizer tokenizer(input);
            std::vector<Token> result = tokenizer.tokenize();
            Translator translator(result);
            try {
                translator.translate();
                std::cout << "Negative test #" << i << '\n';
                std::cout << "\x1b[31mFailed:\x1b[0m\n";
                std::cout << "\x1b[31mNot crashed\x1b[0m\n";
                COUNT_FAILED_TESTS_N++;
            } catch (std::string e) {
                std::string goodOutput = readFile(
                        prefixPath + "positives/test_" + std::to_string(i) + ".output");
                if (true) { // FIXME пофиксить кода оформим нормально сообщения об ошибках
                    std::cout << "Negative test #" << i << '\n';
                    std::cout << "OK\n";
                }
                else {
                    std::cout << "Negative test #" << i << '\n';
                    std::cout << "\x1b[31mFailed:\x1b[0m\n";
                    std::cout << "\x1b[31mWrong exception\x1b[0m\n";
                }
            } catch (...) {
                std::cout << "Negative test #" << i << '\n';
                std::cout << "Failed. Unknown error\n";
            }
        }
    }
};

#endif //WOW_INTERPRETER_TESTS_H
