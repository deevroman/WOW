#ifndef WOW_TOKENIZER_TESTS_H
#define WOW_TOKENIZER_TESTS_H

#include <iostream>
#include <vector>
#include "../../fread/fread.h"
#include "../../tokenizer/tokenizer.h"

#include "../../../external/ThorsSerializer/ThorSerialize/Traits.h"
#include "../../../external/ThorsSerializer/ThorSerialize/JsonThor.h"
#include "../../../external/ThorsSerializer/ThorSerialize/SerUtil.h"

class TokenizerTests {
public:
    void run() {
        run_tests();
    }

    TokenizerTests() {
        loadCountTests();
    }

private:
    int COUNT_TESTS_P{};
    int COUNT_TESTS_N{};

    int COUNT_FAILED_TESTS_P = 0;
    int COUNT_FAILED_TESTS_N = 0;

    void loadCountTests() {
        COUNT_TESTS_P = stoi(readFile("../tests/tokenizer_tests/positives/countTests"));
        COUNT_TESTS_N = stoi(readFile("../tests/tokenizer_tests/negatives/countTests"));
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
            std::string input = readFile("../tests/tokenizer_tests/positives/test_" + std::to_string(i) + ".input");
            Tokenizer tokenizer(input);
            std::vector<Token> result;
            try {
                result = tokenizer.tokenize();
                std::string goodOutput = readFile(
                        "../tests/tokenizer_tests/positives/test_" + std::to_string(i) + ".output");
                auto goodResult = new std::vector<Token>();
                std::stringstream(goodOutput) >> ThorsAnvil::Serialize::jsonImport(*goodResult);
                if (result == goodResult) {
                    std::cout << "Positive test #" << i << '\n'
                              << "OK\n";
                }
                else {
                    std::cout << "Positive test #" << i << '\n'
                              << "\x1b[31mFailed:\x1b[0m\n";
                    COUNT_FAILED_TESTS_P++;
                }
            } catch (...) {
                std::cout << "Positive test #" << i << '\n'
                          << "\x1b[31mFailed:\x1b[0m\n"
                          << "\x1b[31mCrashed\x1b[0m\n";
                COUNT_FAILED_TESTS_P++;
            }
        }
        COUNT_FAILED_TESTS_N = 0;
        for (int i = 1; i <= COUNT_TESTS_N; i++) {
            std::string input = readFile("../tests/tokenizer_tests/negatives/test_" + std::to_string(i) + ".input");
            Tokenizer tokenizer(input);
            std::vector<Token> result;
            try {
                result = tokenizer.tokenize();
                std::cout << "Negative test #" << i << '\n'
                          << "\x1b[31mFailed:\x1b[0m\n"
                          << "\x1b[31mNot crashed\x1b[0m\n";
                COUNT_FAILED_TESTS_N++;
            } catch (...) {
                std::cout << "Negative test #" << i << '\n'
                          << "OK\n";
            }
        }
    }
};


#endif //WOW_TOKENIZER_TESTS_H
