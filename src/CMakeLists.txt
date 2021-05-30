cmake_minimum_required(VERSION 3.16)
project(WOW)

set(CMAKE_CXX_STANDARD 14)

add_executable(WOW
        interpreter/WOW.cpp
        utils/poliz.h
        interpreter/vm.h
        translator/translator.h
        utils/wowobj.h
        interpreter/interpreter.h
        utils/str_split.h)

set(EXECUTABLE_OUTPUT_PATH "../bin/${CMAKE_SYSTEM}")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_${CMAKE_SYSTEM}")
