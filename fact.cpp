//
// Created by blackgeorge on 5/4/19.
//

#include <iostream>
#include <fstream>

#include "serial/cell.h"
#include "serial/peg_elements.h"
#include "serial/peg.h"
#include "serial/grammar_meta.h"
#include "serial/serial_packrat.h"
#include "serial/serial_tree_packrat.h"
#include "serial/tree_node.h"
#include "serial/peg_factory.h"

int NonTerminal::num = 0;
int TreeNode::num = 0;

int main()
{
    Meta meta;
    std::cout << "\n Meta: \n" << meta << "\n";

    std::ifstream ifs("peg_examples/SimpleCalc.txt", std::ifstream::in);
    if (!ifs) std::cout << "Error opening file";
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );

    SerialTreePackrat sp(content, meta);

    auto res = sp.visit(meta);

    if (res)
        std::cout << "Parse successful! \n";
    else
        std::cout << "Syntax Error... \n";
    printTree("", sp.get_root(), true);

    NonTerminal::reset_idx();

    PEGFactory f;
    PEG* calc = f.from_tree(sp.get_root());
    std::cout << *calc;

//    auto start = calc->get_non_term(0);
//    calc->set_start(start);
//
//    SerialPackrat sp2("(0+1)*0+1", *calc);
//    res = sp2.visit(*calc);
//
//    if (res)
//        std::cout << "Parse successful! \n";
//    else
//        std::cout << "Syntax Error... \n";
//
//    sp2.print_cells();
//    printTree("", sp2.get_root(), true);

    return 0;

}


