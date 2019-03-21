cmake_minimum_required(VERSION 3.12)
project(parallel_packrat)

set(CMAKE_CXX_STANDARD 14)

add_executable(parallel_packrat main.cpp)
add_executable(visitor visitor_example.cpp)
add_executable(serial main.cpp serial/cell.cpp serial/cell.h serial/peg_elements.cpp serial/peg_elements.h serial/serial_packrat.h serial/serial_packrat.cpp)
