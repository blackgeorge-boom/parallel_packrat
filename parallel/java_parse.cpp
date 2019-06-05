//
// Created by blackgeorge on 5/10/19.
//

#include <iostream>
#include <fstream>
#include <chrono>

#include "../packrat_cell/cell.h"
#include "../peg/peg_elements.h"
#include "../peg/peg.h"
#include "../syntax_tree/tree_node.h"
#include "../meta_grammar/meta_grammar.h"
#include "../peg_factory/peg_factory.h"
#include "../serial/serial_tree_packrat.h"
#include "simple_parallel.h"

int NonTerminal::num = 0;
int TreeNode::num = 0;

int main()
{
    Meta meta;

    std::ifstream ifs("/home/blackgeorge/documents/thesis/code/parallel_packrat/parallel/Java1.5.txt", std::ifstream::in);
    if (!ifs) std::cout << "Error opening file";
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );

    SerialTreePackrat sp(content, meta);

    auto res = sp.visit(meta);

    if (res)
        std::cout << "Parse successful! \n";
    else
        std::cout << "Syntax Error... \n";

    NonTerminal::reset_idx();

    PEGFactory f;

    PEG* java = f.from_tree(sp.get_root());

    auto start = java->get_non_term(0);
    java->set_start(start);

    std::ifstream ifs2("/home/blackgeorge/documents/thesis/code/parallel_packrat/test/java/Arrays.java", std::ifstream::in);
    if (!ifs2) std::cout << "Error opening file";
    std::string java_file( (std::istreambuf_iterator<char>(ifs2) ),
                           (std::istreambuf_iterator<char>()     ) );

    SimpleParallel sp2(java_file, *java);

    using namespace std::chrono;
    auto t0 = high_resolution_clock::now();
    res = sp2.visit(*java);
    auto tf = high_resolution_clock::now();

    if (res)
        std::cout << "Parse successful! \n";
    else
        std::cout << "Syntax Error... \n";

    std::cout << "done " << duration_cast<milliseconds>(tf-t0).count() << " ms";
}


