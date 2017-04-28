cmake_minimum_required(VERSION 3.6)
project(test_Clion)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES test.c)
add_executable(test_Clion ${SOURCE_FILES})