cmake_minimum_required(VERSION 3.13)
project(WOW)

set(CMAKE_CXX_STANDARD 17)

add_executable(WOW
        interpreter/WOW.cpp
        utils/poliz.h
        interpreter/vm.h
        translator/translator.h
        utils/wowobj.h
        interpreter/interpreter.h
        utils/str_split.h)

SET(EXECUTABLE_OUTPUT_PATH "../bin")