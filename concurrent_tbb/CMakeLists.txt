cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

list(APPEND CMAKE_MODULE_PATH "/home/blackgeorge/CLionProjects/FindTBB")

find_package(TBB COMPONENTS tbbmalloc tbbmalloc_proxy tbb_preview)
find_package(Threads)
find_package(benchmark REQUIRED)

add_executable(elastic_conc parse.cpp ../packrat_cell/cell.cpp ../packrat_cell/cell.h ../peg/peg_elements.cpp
               ../peg/peg_elements.h ../serial/serial_packrat.h ../serial/serial_packrat.cpp
               ../meta_grammar/meta_grammar.cpp ../peg/peg.cpp ../peg/peg.h ../peg_visitor/peg_visitor.cpp
               ../peg_visitor/peg_visitor.h ../meta_grammar/meta_grammar.h
               ../serial/serial_tree_packrat.cpp ../serial/serial_tree_packrat.h ../syntax_tree/tree_node.cpp
               ../syntax_tree/tree_node.h ../peg_factory/peg_factory.cpp ../peg_factory/peg_factory.h
               ../packrat_cell/elastic_cell.h ../packrat_cell/elastic_cell.h conc_elastic_base.cpp conc_elastic_base.h
               conc_elastic_packrat.cpp conc_elastic_packrat.h conc_elastic_worker.cpp conc_elastic_worker.h)

add_executable(bench_elastic_conc benchmark.cpp ../packrat_cell/cell.cpp ../packrat_cell/cell.h ../peg/peg_elements.cpp
        ../peg/peg_elements.h ../serial/serial_packrat.h ../serial/serial_packrat.cpp
        ../meta_grammar/meta_grammar.cpp ../peg/peg.cpp ../peg/peg.h ../peg_visitor/peg_visitor.cpp
        ../peg_visitor/peg_visitor.h ../meta_grammar/meta_grammar.h
        ../serial/serial_tree_packrat.cpp ../serial/serial_tree_packrat.h ../syntax_tree/tree_node.cpp
        ../syntax_tree/tree_node.h ../peg_factory/peg_factory.cpp ../peg_factory/peg_factory.h
        ../packrat_cell/elastic_cell.h ../packrat_cell/elastic_cell.h conc_elastic_base.cpp conc_elastic_base.h
        conc_elastic_packrat.cpp conc_elastic_packrat.h conc_elastic_worker.cpp conc_elastic_worker.h)

target_link_libraries(elastic_conc tbb ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(bench_elastic_conc tbb ${CMAKE_THREAD_LIBS_INIT} benchmark::benchmark)
