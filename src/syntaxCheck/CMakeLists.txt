cmake_minimum_required(VERSION 3.13)
project(run_tests)

set(CMAKE_CXX_STANDARD 17)

add_executable(syntaxCheck syntaxCheck.cpp)

SET( EXECUTABLE_OUTPUT_PATH "../../../bin")