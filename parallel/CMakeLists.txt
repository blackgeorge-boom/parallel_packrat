cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "/home/blackgeorge/CLionProjects/FindTBB")

find_package(TBB COMPONENTS tbbmalloc tbbmalloc_proxy tbb_preview)

add_executable(simple_parallel java_parse.cpp simple_parallel.cpp simple_parallel.h ../packrat_cell/cell.cpp ../packrat_cell/cell.h ../peg/peg_elements.cpp ../peg/peg_elements.h ../serial/serial_packrat.h ../serial/serial_packrat.cpp ../meta_grammar/meta_grammar.cpp ../peg/peg.cpp ../peg/peg.h ../peg_visitor/peg_visitor.cpp ../peg_visitor/peg_visitor.h ../meta_grammar/meta_grammar.h ../serial/serial_tree_packrat.cpp ../serial/serial_tree_packrat.h ../syntax_tree/tree_node.cpp ../syntax_tree/tree_node.h ../peg_factory/peg_factory.cpp ../peg_factory/peg_factory.h locked_cell.cpp locked_cell.h)
add_executable(simple_parallel_calc calc_parse.cpp simple_parallel.cpp simple_parallel.h ../packrat_cell/cell.cpp ../packrat_cell/cell.h ../peg/peg_elements.cpp ../peg/peg_elements.h ../serial/serial_packrat.h ../serial/serial_packrat.cpp ../meta_grammar/meta_grammar.cpp ../peg/peg.cpp ../peg/peg.h ../peg_visitor/peg_visitor.cpp ../peg_visitor/peg_visitor.h ../meta_grammar/meta_grammar.h ../serial/serial_tree_packrat.cpp ../serial/serial_tree_packrat.h ../syntax_tree/tree_node.cpp ../syntax_tree/tree_node.h ../peg_factory/peg_factory.cpp ../peg_factory/peg_factory.h locked_cell.cpp locked_cell.h)

target_link_libraries(simple_parallel tbb)
target_link_libraries(simple_parallel_calc tbb)
