#include <iostream>

#include "serial/cell.h"
#include "serial/peg_elements.h"
#include "serial/peg.h"
#include "serial/serial_packrat.h"

int NonTerminal::num = 0;

int main()
{
    // Grammar
    PEG g;

    // Terminals
    Terminal t0("0");
    Terminal t1("1");
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
    CompositeExpression decExp('/');
    decExp.push_expr(&t0);
    decExp.push_expr(&t1);

    g.push_rule(&dec, &decExp);

    // Primary <- '(' Additive ')' / Decimal
    CompositeExpression primSub1('\b');
    primSub1.push_expr(&lp);
    primSub1.push_expr(&add);
    primSub1.push_expr(&rp);

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

    std::cout << "Grammar: \n";
    std::cout << g;

    SerialPackrat sp("0*(0+1)", g);
    sp.print_cells();
    auto res = sp.visit(g);

    if (res)
        std::cout << "Parse successful! \n";
    else
        std::cout << "Syntax Error... \n";

    sp.print_cells();

    NonTerminal::reset_idx();

    PEG meta;

    std::cout << "Meta Grammar Before: \n";
    std::cout << meta;

    PEG::get_meta(meta);

    std::cout << "Meta Grammar: \n";
    std::cout << meta;

    return 0;
}