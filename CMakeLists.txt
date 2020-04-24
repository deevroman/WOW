cmake_minimum_required(VERSION 3.13)
project(WOW)

set(CMAKE_CXX_STANDARD 17)

add_executable(WOW src/interpretator/WOW.cpp src/utils/poliz.h src/utils/simpleFunctions.h src/interpretator/vm.h src/translator/translator.h src/utils/wowobj.h)

SET(EXECUTABLE_OUTPUT_PATH "../bin")
