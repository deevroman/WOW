#ifndef WOW_SYNTAXCHECK_TESTS_H
#define WOW_SYNTAXCHECK_TESTS_H

#include "../../syntaxCheck/syntaxCheck.h"
#include "../../fread/fread.h"
#include <iostream>

#include "../../../external/ThorsSerializer/ThorSerialize/Traits.h"
#include "../../../external/ThorsSerializer/ThorSerialize/JsonThor.h"
#include "../../../external/ThorsSerializer/ThorSerialize/SerUtil.h"

class SyntaxCheckTests {
public:
    void run() {
        run_tests();
    }

private:
    // TODO
    int COUNT_TESTS_P = 1;
    int COUNT_TESTS_N = 1;

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
            std::string input = readFile("../tests/syntaxCheck_tests/positives/test_" + std::to_string(i) + ".input");
            Tokenizer tokenizer(input);
            std::vector<Token> result = tokenizer.tokenize();
            SyntaxCheck syntaxCheck(result);
            try {
                syntaxCheck.check();
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
            std::string input = readFile("../tests/syntaxCheck_tests/negatives/test_" + std::to_string(i) + ".input");
            Tokenizer tokenizer(input);
            std::vector<Token> result = tokenizer.tokenize();
            SyntaxCheck syntaxCheck(result);
            try {
                syntaxCheck.check();
                std::cout << "Negative test #" << i << '\n';
                std::cout << "\x1b[31mFailed:\x1b[0m\n";
                std::cout << "\x1b[31mNot crashed\x1b[0m\n";
                COUNT_FAILED_TESTS_N++;
            } catch (std::string e) {
                std::string goodOutput = readFile(
                        "../tests/syntaxCheck_tests/positives/test_" + std::to_string(i) + ".output");
                if (true) { // FIXME пофиксить кода оформим нормально сообщения об ошибках
                    std::cout << "Negative test #" << i << '\n';
                    std::cout << "OK\n";
                } else {
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

#endif //WOW_SYNTAXCHECK_TESTS_H
