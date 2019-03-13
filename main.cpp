#include <iostream>

#include "serial/cell.h"
#include "serial/peg_elements.h"

int NonTerminal::num = 0;

int main()
{
    Cell c;
    std::cout << c << '\n';
    NonTerminal nt;
    std::cout << nt << '\n';
    NonTerminal nt2;
    std::cout << nt2 << '\n';
    Terminal t;
    std::cout << t << '\n';

    Expression* list[] = { new NonTerminal("A"), new Terminal(std::string("B")) };

    SerialVisitor sv;
    list[0]->accept(sv);
    list[1]->accept(sv);

    std::string y = std::string("yo");
    CompositeExpression ce(y, 'c');
    ce.push_expr(list[0]);
    ce.push_expr(list[1]);
    std::cout << ce;


    return 0;
}