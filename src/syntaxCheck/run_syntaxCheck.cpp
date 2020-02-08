#include <iostream>
#include "../fread/fread.h"
#include "../tokenizer/tokenizer.h"
#include "../syntaxCheck/syntaxCheck.h"


#include "../../external/ThorsSerializer/ThorSerialize/Traits.h"
#include "../../external/ThorsSerializer/ThorSerialize/JsonThor.h"
#include "../../external/ThorsSerializer/ThorSerialize/SerUtil.h"

int main() {
    using ThorsAnvil::Serialize::jsonExport;
    std::string input = readFile("../src/syntaxCheck/test.input");
    Tokenizer tokenizer(input);
    auto tokens = tokenizer.tokenize();
    try {
        SyntaxCheck syntaxChecker(tokens);
        syntaxChecker.check();
        std::cout << "OK\nSave how test?\n0 - No\n1 - Yes\n";
        int saveTest;
        std::cin >> saveTest;
        if (saveTest) {
            int cntTests = stoi(readFile("../tests/syntaxCheck_tests/positives/countTests"));
            freopen("../tests/syntaxCheck_tests/positives/countTests", "w", stdout);
            std::cout << ++cntTests;
            std::string inputPath = "../tests/syntaxCheck_tests/positives/test_" + std::to_string(cntTests) + ".input";
            freopen(inputPath.c_str(), "w", stdout);
            std::cout << input;
        }
    } catch (std::string e) {
        std::cerr << e;
        std::cout << "\nSave how test?\n0 - No\n1 - Yes\n";
        std::string inputPath = "../src/syntaxCheck/test.output";
        freopen(inputPath.c_str(), "w", stdout);
        std::cout << e;
        int saveTest;
        std::cin >> saveTest;
        if (saveTest) {
            int cntTests = stoi(readFile("../tests/syntaxCheck_tests/negatives/countTests"));
            freopen("../tests/syntaxCheck_tests/negatives/countTests", "w", stdout);
            std::cout << ++cntTests;
            std::string inputPath =
                    "../tests/syntaxCheck_tests/negatives/test_" + std::to_string(cntTests) + ".input";
            freopen(inputPath.c_str(), "w", stdout);
            std::cout << input;
            std::string outputPath = "../tests/syntaxCheck_tests/negatives/test_" + std::to_string(cntTests) + ".output";
            freopen(outputPath.c_str(), "w", stdout);
            std::cout << jsonExport(tokens);
        }
    }

    return 0;
}