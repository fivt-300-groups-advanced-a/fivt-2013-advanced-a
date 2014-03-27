cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./../bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "./../bin/")
project(Graph)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(SOURCE_EXE main.cpp)

set(SOURCE_LIB graph.cc)

add_library(graph SHARED ${SOURCE_LIB})

add_executable(main ${SOURCE_EXE})

find_package(GTest REQUIRED)
if(NOT GTEST_FOUND)
    message(SEND_ERROR "Failed to find GTEST")
    return()
else()
    include_directories(${GTEST_INCLUDE_DIR})
endif()


target_link_libraries(main graph ${GTEST_LIBRARIES})
