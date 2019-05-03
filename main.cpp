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

int main()
{
    // Grammar
    PEG g;

    // Terminals
    Terminal lp("(");
    Terminal rp(")");
    Terminal times("*");
    Terminal plus("+");
    AnyChar ac;

    // Non Terminals
    NonTerminal ae("ArithmeticExpression");
    NonTerminal add("Additive");
    NonTerminal mult("Multitive");
    NonTerminal prim("Primary");
    NonTerminal dec("Decimal");

    // Rules

    // Decimal <- '0' / '1'
    CompositeExpression decExp('/', "0123456789");

    g.push_rule(&dec, &decExp);

    // Primary <- '(' Additive ')' / Decimal
    CompositeExpression primSub1('\b', {&lp, &add, &rp});
    CompositeExpression primExp('/');
    primExp.push_expr(&primSub1);
    primExp.push_expr(&dec);

    g.push_rule(&prim, &primExp);

    // Multitive <- Primary '*' Multitive / Primary
    CompositeExpression multSub1('\b');
    multSub1.push_expr(&prim);
    multSub1.push_expr(&times);
    multSub1.push_expr(&mult);

    CompositeExpression multExp('/');
    multExp.push_expr(&multSub1);
    multExp.push_expr(&prim);

    g.push_rule(&mult, &multExp);

    // Additive <- Multitive '+' Additive / Multitive
    CompositeExpression addSub1('\b');
    addSub1.push_expr(&mult);
    addSub1.push_expr(&plus);
    addSub1.push_expr(&add);

    CompositeExpression addExp('/');
    addExp.push_expr(&addSub1);
    addExp.push_expr(&mult);

    g.push_rule(&add, &addExp);

    // ArithmeticExpression <- Additive !.
    CompositeExpression aeSub2('!');
    aeSub2.push_expr(&ac);

    CompositeExpression aeExp('\b');
    aeExp.push_expr(&add);
    aeExp.push_expr(&aeSub2);

    g.push_rule(&ae, &aeExp);

    // Set starting rule
    g.set_start(&ae);

    std::cout << "\nGrammar: \n";
    std::cout << g;

    SerialPackrat sp("(3+2)*7+1", g);

    auto res = sp.visit(g);

    if (res)
        std::cout << "Parse successful! \n";
    else
        std::cout << "Syntax Error... \n";
    sp.print_cells();

    NonTerminal::reset_idx();

    Meta meta;
    // std::cout << "\n Meta: \n" << meta << "\n";

    std::ifstream ifs("peg_examples/Calc.txt", std::ifstream::in);
    if (!ifs) std::cout << "Error opening file";
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );

    // std::cout << "\nInput: \n" << content << "\n";

    SerialTreePackrat sp2(content, meta);

    res = sp2.visit(meta);

    if (res)
        std::cout << "Parse successful! \n";
    else
        std::cout << "Syntax Error... \n";
    printTree("", sp2.get_root(), true);

    SerialTreePackrat sp3("7+1", g);
    res = sp3.visit(g);

    if (res)
        std::cout << "Parse successful! \n";
    else
        std::cout << "Syntax Error... \n";

    sp3.print_cells();
    printTree("", sp3.get_root(), true);

    PEGFactory f;
    PEG* calc = f.from_tree(sp3.get_root());
    std::cout << *calc;

    return 0;
}