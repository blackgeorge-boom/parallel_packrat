//
// Created by blackgeorge on 5/10/19.
//

#include <iostream>
#include <fstream>
#include <chrono>

#include "../packrat_cell/cell.h"
#include "../peg/peg.h"
#include "../syntax_tree/tree_node.h"
#include "../meta_grammar/meta_grammar.h"
#include "../peg_factory/peg_factory.h"
#include "../serial/serial_tree_packrat.h"
#include "parallel_packrats.h"

int NonTerminal::num = 0;
int TreeNode::num = 0;

int main()
{
    Meta meta;

    std::ifstream ifs("test/peg_examples/SimpleCalc.txt", std::ifstream::in);
    if (!ifs) std::cout << "Error opening file";
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );

    SerialTreePackrat sp(content, meta);

    auto res = sp.visit(meta);

    if (res)
        std::cout << "Parse successful!" << std::endl;
    else
        std::cout << "Syntax Error..." << std::endl;

    NonTerminal::reset_idx();

    PEGFactory f;

    PEG* calc = f.from_tree(sp.get_root());

    auto start = calc->get_non_term(0);
    calc->set_start(start);

    std::ifstream ifs2("test/calc/simple.txt", std::ifstream::in);
    if (!ifs2) std::cout << "Error opening file";
    std::string calc_file( (std::istreambuf_iterator<char>(ifs2) ),
                           (std::istreambuf_iterator<char>()     ) );
    TableParallel sp2(calc_file, *calc);

    using namespace std::chrono;
    auto t0 = high_resolution_clock::now();
    res = sp2.visit(*calc);
    auto tf = high_resolution_clock::now();

    if (res)
        std::cout << "Parse successful!" << std::endl;
    else
        std::cout << "Syntax Error..." << std::endl;

    std::cout << "done " << duration_cast<milliseconds>(tf-t0).count() << " ms";
}


