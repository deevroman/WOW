#include <iostream>
#include <vector>
#include <string>
#include "../utils/fread.h"
#include "tokenizer.h"
#include <fstream>

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
            std::fstream testCountFile("../tests/tokenizer_tests/positives/countTests");
            testCountFile << ++cntTests;

            std::string inputPath = "../tests/tokenizer_tests/positives/test_" + std::to_string(cntTests) + ".input";
            std::ofstream testInputFile(inputPath);
            testInputFile << input;
            std::string outputPath = "../tests/tokenizer_tests/positives/test_" + std::to_string(cntTests) + ".output";
            std::ofstream testOutputFile(outputPath);
            testOutputFile << jsonExport(tokens);
            std::cout << "\nTest #" + std::to_string(cntTests) + " saved" << std::flush;
        }
    } catch (std::string e) {
        std::cerr << e;
        std::cout << "Save how test?\n0 - No\n1 - Yes\n";
        int saveTest;
        std::cin >> saveTest;
        if (saveTest) {
            int cntTests = stoi(readFile("../tests/tokenizer_tests/negatives/countTests"));
            std::ofstream testCountFile("../tests/tokenizer_tests/negatives/countTests");
            testCountFile << ++cntTests;
            std::string inputPath =
                    "../tests/tokenizer_tests/negatives/test_" + std::to_string(cntTests) + ".input";
            std::ofstream testInputFile(inputPath);
            testInputFile << e;
            std::cout << "Test \n#" + std::to_string(cntTests) + " saved";
        }
    }
    return 0;
}