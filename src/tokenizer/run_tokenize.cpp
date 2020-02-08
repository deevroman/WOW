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
        freopen("../src/tokenizer/test.output", "w", stderr);
        std::cerr << jsonExport(tokens);

        std::cout << "Save how test?\n0 - No\n1 - Yes\n";
        int saveTest;
        std::cin >> saveTest;
        if (saveTest) {
            int cntTests = stoi(readFile("../tests/tokenizer_tests/positives/countTests"));
            freopen("../tests/tokenizer_tests/positives/countTests", "w", stdout);
            std::cout << ++cntTests;

            std::string inputPath = "../tests/tokenizer_tests/positives/test_" + std::to_string(cntTests) + ".input";
            freopen(inputPath.c_str(), "w", stdout);
            std::cout << input;
            std::string outputPath = "../tests/tokenizer_tests/positives/test_" + std::to_string(cntTests) + ".output";
            freopen(outputPath.c_str(), "w", stdout);
            std::cout << jsonExport(tokens);
        }
    } catch (std::string e) {
        std::cerr << e;
        std::cout << "Save how test?\n0 - No\n1 - Yes\n";
        int saveTest;
        std::cin >> saveTest;
        if (saveTest) {
            int cntTests = stoi(readFile("../tests/tokenizer_tests/negatives/countTests"));
            freopen("../tests/tokenizer_tests/negatives/countTests", "w", stdout);
            std::cout << ++cntTests;
            std::string inputPath =
                    "../tests/tokenizer_tests/negatives/test_" + std::to_string(cntTests) + ".input";
            freopen(inputPath.c_str(), "w", stdout);
            std::cout << input;
        }
    }
    return 0;
}