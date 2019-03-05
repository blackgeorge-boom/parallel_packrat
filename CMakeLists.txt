cmake_minimum_required(VERSION 3.12)
project(parallel_packrat)

set(CMAKE_CXX_STANDARD 14)

add_executable(parallel_packrat main.cpp)
add_executable(visitor visitor_example.cpp)
