#include <iostream>

#include "serial/cell.h"
#include "serial/peg_elements.h"

int NonTerminal::num = 0;

int main()
{
    Cell c;
    std::cout << c << '\n';
    NonTerminal nt("A");
    std::cout << nt << '\n';
    NonTerminal nt2("B");
    std::cout << nt2 << '\n';
    Terminal t("identifier");
    std::cout << t << '\n';
    Empty e;
    std::cout << e << '\n';
    AnyChar ac;
    std::cout << ac << '\n';


    Expression* list[4] = { &nt, &t, &e, &ac };

    SerialVisitor sv;
    list[0]->accept(sv);
    list[1]->accept(sv);
    list[2]->accept(sv);
    list[3]->accept(sv);

    std::vector<Expression*> v = { &nt, &nt2, &t, &e, &ac };
    CompositeExpression ce('/');
    CompositeExpression ce2('\b', std::move(v));
    ce.push_expr(list[0]);
    ce.push_expr(list[1]);
    std::cout << ce << "\n";
    std::cout << ce2 << "\n";

    PEG g;
    g.set_start(&nt2);
    g.push_rule(&nt, &ce);
    g.push_rule(&nt2, &ce2);
    std::cout << "Grammar: \n";
    std::cout << g << "\n";
    std::cout << ce << "\n";
    NonTerminal* x = g.get_start();


    return 0;
}