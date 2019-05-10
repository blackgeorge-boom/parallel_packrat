//
// Created by blackgeorge on 5/10/19.
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
    PEG g;

    Terminal forwardSlash("/");

    NonTerminal expression("Expression");
    NonTerminal sequence("Sequence");
    NonTerminal slash("Slash");

    // Expression <- Sequence (SLASH Sequence)*                     # Type 2
    CompositeExpression expressionExp('\b');
    expressionExp.push_expr(&sequence);
    CompositeExpression expressionSubExp('\b', {&slash, &sequence});
    expressionExp.push_expr(new CompositeExpression('*', {&expressionSubExp}));
    g.push_rule(&expression, &expressionExp);

    // Sequence <- Prefix*                                          # Type 3
    static CompositeExpression sequenceExp('*', "a");
    g.push_rule(&sequence, &sequenceExp);

    static CompositeExpression slashExp('\b', {&forwardSlash});
    g.push_rule(&slash, &slashExp);

    g.set_start(&expression);

    std::cout << g << "\n";

    SerialTreePackrat sp("a/a", g);

    auto res = sp.visit(g);

    if (res)
        std::cout << "Parse successful! \n";
    else
        std::cout << "Syntax Error... \n";
    printTree("", sp.get_root(), true);

//    NonTerminal::reset_idx();
//
//    PEGFactory f;
//    PEG* calc = f.from_tree(sp.get_root());
//    std::cout << *calc;
}