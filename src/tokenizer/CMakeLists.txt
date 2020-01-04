cmake_minimum_required(VERSION 3.13)
project(tokenizer)

set(CMAKE_CXX_STANDARD 17)

add_executable(tokenizer run_tokenize.cpp)

SET( EXECUTABLE_OUTPUT_PATH "../../../bin")