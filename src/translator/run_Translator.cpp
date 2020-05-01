#include <iostream>
#include "../fread/fread.h"
#include "../tokenizer/tokenizer.h"
#include "../translator/translator.h"


#include "../../external/ThorsSerializer/ThorSerialize/Traits.h"
#include "../../external/ThorsSerializer/ThorSerialize/JsonThor.h"
#include "../../external/ThorsSerializer/ThorSerialize/SerUtil.h"

int main() {
    using ThorsAnvil::Serialize::jsonExport;
    std::string input = readFile("../src/translator/test.input");
    Tokenizer tokenizer(input);
    std::vector<Token> tokens;
    try {
        tokens = tokenizer.tokenize();
    } catch (std::string e) {
        std::cout << "Not parsed\n";
        std::cerr << e;
        return 0;
    }
    try {
        Translator syntaxChecker(tokens);
        syntaxChecker.translate(); // check()
        std::cout << "OK\nSave how test?\n0 - No\n1 - Yes\n";
        int saveTest;
        std::cin >> saveTest;
        if (saveTest) {
            int cntTests = stoi(readFile("../tests/syntaxCheck_tests/positives/countTests"));
            std::ofstream testCountFile("../tests/syntaxCheck_tests/positives/countTests");
            testCountFile << ++cntTests;

            std::ofstream testInputFile("../tests/syntaxCheck_tests/positives/test_"+ std::to_string(cntTests) + ".input");
            testInputFile << input;
            std::cout << "Test \n#" + std::to_string(cntTests) + " saved";
        }
    } catch (std::string e) {
        std::cerr << e;
        std::cout << "\nSave how test?\n0 - No\n1 - Yes\n";
        std::ofstream testNotSaveOutputFile("../src/translator/test.output");
        testNotSaveOutputFile << e;
        int saveTest;
        std::cin >> saveTest;
        if (saveTest) {
            int cntTests = stoi(readFile("../tests/syntaxCheck_tests/negatives/countTests"));
            std::ofstream testCountFile("../tests/syntaxCheck_tests/negatives/countTests");
            testCountFile << ++cntTests;
            std::string inputPath =
                    "../tests/syntaxCheck_tests/negatives/test_" + std::to_string(cntTests) + ".input";
            std::ofstream testInputFile(inputPath);
            testInputFile << input;

            std::string outputPath =
                    "../tests/syntaxCheck_tests/negatives/test_" + std::to_string(cntTests) + ".output";
            std::ofstream testOutputFile(outputPath);
            testOutputFile << e;

            std::cout << "Test \n#" + std::to_string(cntTests) + " saved";
        }
    }

    return 0;
}