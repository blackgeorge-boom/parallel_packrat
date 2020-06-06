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
#include "elastic_packrat.h"

int NonTerminal::num = 0;
int TreeNode::num = 0;

int main(int argc, char** argv)
{
    std::string grammar_def(argv[1]);
    std::string file_to_parse(argv[2]);

    Meta meta;

    std::ifstream ifs(grammar_def, std::ifstream::in);
    if (!ifs) {
        std::cout << "Error opening file";
        return 0;
    }

    std::string content((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());

    SerialTreePackrat sp(content, meta);

    auto res = sp.visit(meta);

    if (res)
        std::cout << "Generating grammar successful!" << std::endl;
    else {
        std::cout << "Syntax Error..." << std::endl;
        return 0;
    }

    NonTerminal::reset_idx();

    PEGFactory f;

    PEG* grammar = f.from_tree(sp.get_root());

    auto start = grammar->get_non_term(0);
    grammar->set_start(start);

    std::ifstream ifs2(file_to_parse, std::ifstream::in);
    if (!ifs2) {
        std::cout << "Error opening file";
        return 0;
    }

    std::string input( (std::istreambuf_iterator<char>(ifs2) ),
                           (std::istreambuf_iterator<char>()     ) );

    SerialPackrat sp2(input, *grammar);

    using namespace std::chrono;
    auto t0 = high_resolution_clock::now();
    res = sp2.visit(*grammar);
    auto tf = high_resolution_clock::now();

    if (res)
        std::cout << "Serial parsing successful!" << std::endl;
    else
        std::cout << "Syntax Error..." << std::endl;

    std::cout << "  in : " << duration_cast<milliseconds>(tf-t0).count() << " ms" << std::endl;

    Elastic sp3(input, *grammar, 256, 32); // TODO: Benchmark window size

    t0 = high_resolution_clock::now();
    res = sp3.visit(*grammar);
    tf = high_resolution_clock::now();

    if (res)
        std::cout << "Elastic parsing successful!" << std::endl;
    else
        std::cout << "Syntax Error..." << std::endl;

    std::cout << "  in : " << duration_cast<milliseconds>(tf-t0).count() << " ms" << std::endl;

    return 0;
}
