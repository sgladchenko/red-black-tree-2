cmake_minimum_required(VERSION 3.14)
project(red_black_tree_2_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(red_black_tree_2_test test.cpp perf.cpp)
