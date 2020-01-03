#ifndef WOW_TOKENIZER_TESTS_H
#define WOW_TOKENIZER_TESTS_H

#include "../../tokenizer/tokenizer.h"
#include <fstream>
#include <iostream>

class TokenizerTests {
public:
    int run(){
        int failed = 0;
        failed += run_positives();
        failed += run_negatives();
        return failed;
    }

private:
    const COUNT_TEST_P = 1;
    int run_positives(){
        int cnt_wrong = 0;
        for(int i = 0; i < CONUT_TEST_P; i++) {
            ifstream input("tests/test1.input");
            // TODO readfile
            Tokenizer tokenizer;
            // TODO load good result
            int failed = 0; // упал ли токенизатор
            std::vector <std::string> result;
            try {
                result = tokenizer.tokenize();
            } catch (...) {

            }
        }
        return cnt_wrong;
    }
    int run_negatives(){

    }
};


#endif //WOW_TOKENIZER_TESTS_H
