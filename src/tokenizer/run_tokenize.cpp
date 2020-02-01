#include <iostream>
#include <vector>
#include <string>
#include "../fread/fread.h"
#include "tokenizer.h"

#include "../../external/ThorsSerializer/ThorSerialize/Traits.h"
#include "../../external/ThorsSerializer/ThorSerialize/JsonThor.h"
#include "../../external/ThorsSerializer/ThorSerialize/SerUtil.h"

int main() {
    using ThorsAnvil::Serialize::jsonExport;
    std::string input = readFile("../src/tokenizer/test.input");
    Tokenizer tokenizer(input);
    try {
        std::vector<Token> tokens = tokenizer.tokenize();
        std::cout << "Save how test?\n0 - No\nOther - Number test\n";
        int saveTest;
        std::cin >> saveTest;
        if (saveTest > 0) {
            std::string inputPath = "../tests/tokenizer_tests/positives/test_" + std::to_string(saveTest) + ".input";
            freopen(inputPath.c_str(), "w", stdout);
            std::cout << input;
            std::string outputPath = "../tests/tokenizer_tests/positives/test_" + std::to_string(saveTest) + ".output";
            freopen(outputPath.c_str(), "w", stdout);
            std::cout << jsonExport(tokens);
        } else if (saveTest == 0) {
            freopen("../src/tokenizer/test.output", "w", stdout);
            std::cout << jsonExport(tokens);
        } else {
            std::string inputPath = "../tests/tokenizer_tests/negatives/test_" + std::to_string(saveTest) + ".input";
            freopen(inputPath.c_str(), "w", stdout);
            std::cout << input;
        }
    } catch (std::string e) {
        std::cerr << e;
        std::cout << "Save how test?\n0 - No\nOther - Number test\n";
        int saveTest;
        std::cin >> saveTest;
        if (saveTest < 0) {
            std::string inputPath =
                    "../tests/tokenizer_tests/negatives/test_" + std::to_string(abs(saveTest)) + ".input";
            freopen(inputPath.c_str(), "w", stdout);
            std::cout << input;
        }
    }
    return 0;
}