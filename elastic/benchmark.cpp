//
// Created by blackgeorge on 22/3/20.
//

#include <iostream>
#include <fstream>
#include <chrono>

#include <benchmark/benchmark.h>

#include "../packrat_cell/cell.h"
#include "../peg/peg.h"
#include "../syntax_tree/tree_node.h"
#include "../meta_grammar/meta_grammar.h"
#include "../peg_factory/peg_factory.h"
#include "../serial/serial_tree_packrat.h"
#include "elastic_packrat.h"

int NonTerminal::num = 0;
int TreeNode::num = 0;

Meta meta;

static void BM_Packrat(benchmark::State& state) {
    std::string grammar_def("test/peg_examples/Java1.5.txt");
    std::string file_to_parse("test/java/Arrays.java");

    std::ifstream ifs(grammar_def, std::ifstream::in);
    if (!ifs) {
        std::cout << "Error opening file";
    }

    std::string content((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());

    SerialTreePackrat sp(content, meta);

    auto res = sp.visit(meta);

    NonTerminal::reset_idx();

    PEGFactory f;

    PEG* grammar = f.from_tree(sp.get_root());

    auto start = grammar->get_non_term(0);
    grammar->set_start(start);

    std::ifstream ifs2(file_to_parse, std::ifstream::in);
    if (!ifs2) {
        std::cout << "Error opening file";
    }

    std::string input( (std::istreambuf_iterator<char>(ifs2) ),
                       (std::istreambuf_iterator<char>()     ) );

    // Global variables.
    const size_t bigger_than_cachesize = 14 * 1024 * 1024;
    long *p = new long[bigger_than_cachesize];

    for (auto _ : state) {
        state.PauseTiming();
        // When you want to "flush" cache.
        for(int i = 0; i < bigger_than_cachesize; i++)
            p[i] = rand();
        Elastic sp2(input, *grammar, state.range(0), state.range(1));
        state.ResumeTiming();
        sp2.visit(*grammar);
    }
}

// 16/32 seems a good choice for threshold
// 256/512/1024 for window size
static void CustomArguments(benchmark::internal::Benchmark* b) {
    for (int i = 1024; i <= 1024; i *= 2)
        for (int j = 0; j <= 48; j += 16)
            b->Args({i, j});
}

// Register the function as a benchmark
BENCHMARK(BM_Packrat)->Unit(benchmark::kMillisecond)->Apply(CustomArguments);

BENCHMARK_MAIN();