cmake_minimum_required(VERSION 3.14)



add_library(utility STATIC utility.cpp)

target_include_directories(utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})