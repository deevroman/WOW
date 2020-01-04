#ifndef WOW_TOKENIZER_TESTS_H
#define WOW_TOKENIZER_TESTS_H

#include "../../tokenizer/tokenizer.h"
#include "../../fread/fread.h"
#include <iostream>

class TokenizerTests {
public:
    int run(){
        int failed = 0;
        failed += run_tests();
        return failed;
    }

private:
    const int COUNT_TEST_P = 1;
    const int COUNT_TEST_N = 1;
    int run_tests(){
        int cnt_wrong = 0;

        for(int i = 0; i < CONUT_TEST_P; i++) {
            std::string input = readFile("../tests/tokenizer_tests/positives/test_" + std::string(i + '0') +  ".input");
            Tokenizer tokenizer(input);
            std::vector <std::string> result;
            try {
                result = tokenizer.tokenize();
            } catch (...) {

            }
        }

        for(int i = 0; i < CONUT_TEST_N; i++) {
            std::string input = readFile("../tests/tokenizer_tests/negatives/test_" + std::string(i + '0') +  ".input");
            Tokenizer tokenizer(input);
            std::vector <std::string> result;
            try {
                result = tokenizer.tokenize();
            } catch (...) {

            }
        }
        return cnt_wrong;
    }
};


#endif //WOW_TOKENIZER_TESTS_H
