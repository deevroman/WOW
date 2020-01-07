#include <iostream>
#include <vector>
#include <string>
#include "../fread/fread.h"
#include "tokenizer.h"

int main(){
    std::string input = readFile("test.input");
    Tokenizer tokenizer(input);
    try {
        std::vector<Token> tokens = tokenizer.tokenize();
        // TODO
        std::cout << "kek";
    } catch(std::string e){
        std::cerr << e;
    }
    return 0;
}