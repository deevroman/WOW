cmake_minimum_required(VERSION 3.13)
project(WOW)

set(CMAKE_CXX_STANDARD 17)

add_executable(WOW WOW.cpp src/syntaxCheck/run_syntaxCheck.cpp src/syntaxCheck/run_syntaxCheck.h src/syntaxCheck/syntaxCheck.cpp src/syntaxCheck/syntaxCheck.h src/exception/exception.h src/tests/syntaxCheck_tests/syntaxCheck_tests.h)

SET( EXECUTABLE_OUTPUT_PATH "./bin")
