#ifndef WOW_TOKENIZER_TESTS_H
#define WOW_TOKENIZER_TESTS_H

#include "../../tokenizer/tokenizer.h"
#include "../../fread/fread.h"
#include <iostream>

#include "../../../external/ThorsSerializer/ThorSerialize/Traits.h"
#include "../../../external/ThorsSerializer/ThorSerialize/JsonThor.h"
#include "../../../external/ThorsSerializer/ThorSerialize/SerUtil.h"

class TokenizerTests {
public:
    void run() {
        run_tests();
    }


private:
    // TODO
    int COUNT_TESTS_P = 12;
    int COUNT_TESTS_N = 4;

    int COUNT_FAILED_TESTS_P = 0;
    int COUNT_FAILED_TESTS_N = 0;
public:
    int getCountTestsP() const {
        return COUNT_TESTS_P;
    }

    int getCountTestsN() const {
        return COUNT_TESTS_N;
    }

    int getCountTests() const {
        return COUNT_TESTS_P + COUNT_TESTS_N;
    }

    int getCountFailedTestsP() const {
        return COUNT_FAILED_TESTS_P;
    }

    int getCountFailedTestsN() const {
        return COUNT_FAILED_TESTS_N;
    }

    int getCountFailedTests() const {
        return COUNT_FAILED_TESTS_P + COUNT_FAILED_TESTS_N;
    }


private:
    void run_tests() {
        using ThorsAnvil::Serialize::jsonExport;

        COUNT_FAILED_TESTS_P = 0;
        for (int i = 1; i <= COUNT_TESTS_P; i++) {
            std::string input = readFile("../tests/tokenizer_tests/positives/test_" + std::to_string(i) + ".input");
            Tokenizer tokenizer(input);
            std::vector<Token> result;
            try {
                result = tokenizer.tokenize();
                std::string goodOutput = readFile(
                        "../tests/tokenizer_tests/positives/test_" + std::to_string(i) + ".output");
                std::vector<Token> goodResult;
                std::stringstream(goodOutput) >> jsonImport(goodResult);
                if (result == goodResult) {
                    std::cout << "Positive test #" << i << '\n';
                    std::cout << "OK\n";
                } else {
                    std::cout << "Positive test #" << i << '\n';
                    std::cout << "\x1b[31mFailed:\x1b[0m\n";
                    COUNT_FAILED_TESTS_P++;
                }
            } catch (...) {
                std::cout << "Positive test #" << i << '\n';
                std::cout << "\x1b[31mFailed:\x1b[0m\n";
                std::cout << "\x1b[31mCrashed\x1b[0m\n";
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
                std::cout << "Negative test #" << i << '\n';
                std::cout << "\x1b[31mFailed:\x1b[0m\n";
                std::cout << "\x1b[31mNot crashed\x1b[0m\n";
                COUNT_FAILED_TESTS_N++;
            } catch (...) {
                std::cout << "Negative test #" << i << '\n';
                std::cout << "OK\n";
            }
        }
    }
};


#endif //WOW_TOKENIZER_TESTS_H
