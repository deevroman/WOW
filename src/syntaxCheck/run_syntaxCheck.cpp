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
        std::cout << "Save how test?\n0 - No\nOther - Number test\n";
        int saveTest;
        std::cin >> saveTest;
        if (saveTest > 0) {
            std::string inputPath = "../tests/syntaxCheck_tests/positives/test_" + std::to_string(saveTest) + ".input";
            freopen(inputPath.c_str(), "w", stdout);
            std::cout << input;
        } else if (saveTest < 0){
            std::string inputPath = "../tests/syntaxCheck_tests/negatives/test_" + std::to_string(saveTest) + ".input";
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
                    "../tests/syntaxCheck_tests/negatives/test_" + std::to_string(abs(saveTest)) + ".input";
            freopen(inputPath.c_str(), "w", stdout);
            std::cout << input;
            std::string outputPath = "../tests/syntaxCheck_tests/negatives/test_" + std::to_string(saveTest) + ".output";
            freopen(outputPath.c_str(), "w", stdout);
            std::cout << jsonExport(tokens);
        }
    }

    return 0;
}