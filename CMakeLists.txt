cmake_minimum_required(VERSION 3.12)
project(parallel_packrat)

set(CMAKE_CXX_STANDARD 14)

add_executable(parallel_packrat main.cpp)
add_executable(visitor visitor_example.cpp)
add_executable(serial main.cpp serial/cell.cpp serial/cell.h serial/peg_elements.cpp serial/peg_elements.h serial/serial_packrat.h serial/serial_packrat.cpp serial/grammar_meta.cpp serial/peg.cpp serial/peg.h serial/peg_visitor.cpp serial/peg_visitor.h serial/grammar_meta.h serial/serial_tree_packrat.cpp serial/serial_tree_packrat.h serial/tree_node.cpp serial/tree_node.h serial/peg_factory.cpp serial/peg_factory.h)
add_executable(fact fact.cpp serial/cell.cpp serial/cell.h serial/peg_elements.cpp serial/peg_elements.h serial/serial_packrat.h serial/serial_packrat.cpp serial/grammar_meta.cpp serial/peg.cpp serial/peg.h serial/peg_visitor.cpp serial/peg_visitor.h serial/grammar_meta.h serial/serial_tree_packrat.cpp serial/serial_tree_packrat.h serial/tree_node.cpp serial/tree_node.h serial/peg_factory.cpp serial/peg_factory.h)
add_executable(debug debug.cpp serial/cell.cpp serial/cell.h serial/peg_elements.cpp serial/peg_elements.h serial/serial_packrat.h serial/serial_packrat.cpp serial/grammar_meta.cpp serial/peg.cpp serial/peg.h serial/peg_visitor.cpp serial/peg_visitor.h serial/grammar_meta.h serial/serial_tree_packrat.cpp serial/serial_tree_packrat.h serial/tree_node.cpp serial/tree_node.h serial/peg_factory.cpp serial/peg_factory.h)
add_executable(java_parse java_parse.cpp serial/cell.cpp serial/cell.h serial/peg_elements.cpp serial/peg_elements.h serial/serial_packrat.h serial/serial_packrat.cpp serial/grammar_meta.cpp serial/peg.cpp serial/peg.h serial/peg_visitor.cpp serial/peg_visitor.h serial/grammar_meta.h serial/serial_tree_packrat.cpp serial/serial_tree_packrat.h serial/tree_node.cpp serial/tree_node.h serial/peg_factory.cpp serial/peg_factory.h)
