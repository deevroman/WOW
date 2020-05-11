#include <iostream>
#include "../fread/fread.h"
#include "interpreter.h"

using namespace std;

int main() {
    std::string source = readFile("../test.source");

    Runner runner(source, &std::cin, );



    const std::string prefixPath = "../tests/interpreter_tests/";
    int cntTests = stoi(readFile(prefixPath + "positives/countTests"));
    std::ofstream testCountFile(prefixPath + "positives/countTests");
    testCountFile << ++cntTests;



    return 0;
}
