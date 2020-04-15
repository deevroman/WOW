cmake_minimum_required(VERSION 3.13)
project(syntaxCheck)

set(CMAKE_CXX_STANDARD 17)

add_executable(syntaxCheck run_syntaxCheck.cpp)

SET(EXECUTABLE_OUTPUT_PATH "../../../bin")