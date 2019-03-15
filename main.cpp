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

//    Expression* list[] = { new NonTerminal("A"), new Terminal(std::string("B")) };
    Expression* list[2] = { &nt, &t };

    SerialVisitor sv;
    list[0]->accept(sv);
    list[1]->accept(sv);

    CompositeExpression ce("Comp", '/');
    ce.push_expr(list[0]);
    ce.push_expr(list[1]);
    std::cout << ce << "\n";
    std::cout << "yo";

    return 0;
}