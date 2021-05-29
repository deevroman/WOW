cmake_minimum_required(VERSION 3.13)
project(run_tests)

set(CMAKE_CXX_STANDARD 17)

add_executable(run_tests run_tests.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

SET(EXECUTABLE_OUTPUT_PATH "bin")
