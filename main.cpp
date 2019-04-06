#include <iostream>

#include "serial/cell.h"
#include "serial/peg_elements.h"
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
    NonTerminal dec("Decimal");
    NonTerminal prim("Primary");
    NonTerminal mult("Multitive");
    NonTerminal add("Additive");
    NonTerminal ae("ArithmeticExpression");

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
    primExp.push_expr(&decExp);

    g.push_rule(&dec, &decExp);

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

    CompositeExpression aeExp('/');
    aeExp.push_expr(&add);
    aeExp.push_expr(&aeSub2);

    g.push_rule(&ae, &aeExp);

    // Set starting rule
    g.set_start(&add);

    std::cout << "Grammar: \n";
    std::cout << g << "\n";

    SerialPackrat sp("0 + 1", g);
    sp.visit(g);

    return 0;
}