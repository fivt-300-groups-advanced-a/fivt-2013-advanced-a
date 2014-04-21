cmake_minimum_required(VERSION 2.8)

project(graphs)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(GTest REQUIRED)

add_executable(main main.cpp graph.cpp)

add_library(graph STATIC graph.cpp)

target_link_libraries(main graph gtest gtest_main)
