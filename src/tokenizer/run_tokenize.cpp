#include <iostream>
#include "../fread/fread.h"
#include "tokenizer.h"

int main(){
    std::string input = readFile("test.input");
    Tokenizer tokenizer(input);
    tokenizer.tokenize(); // TODO

    return 0;
}