cmake_minimum_required(VERSION 3.13)
project(WOW)

set(CMAKE_CXX_STANDARD 17)

add_executable(WOW
        src/interpreter/WOW.cpp
        src/utils/poliz.h
        src/interpreter/vm.h
        src/translator/translator.h
        src/utils/wowobj.h
        src/interpreter/interpreter.h
        src/utils/str_split.h)

SET(EXECUTABLE_OUTPUT_PATH "bin")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")