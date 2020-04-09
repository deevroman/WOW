cmake_minimum_required(VERSION 3.13)
project(WOW)

set(CMAKE_CXX_STANDARD 17)

add_executable(WOW src/interpetator/WOW.cpp)

SET( EXECUTABLE_OUTPUT_PATH "./bin")
