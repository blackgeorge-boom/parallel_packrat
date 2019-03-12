#include <iostream>

#include "serial/cell.h"
#include "serial/peg_elements.h"

int NonTerminal::num = 0;

int main()
{
    std::cout << "Hello, World!" << std::endl;
    Cell c;
    std::cout << c << '\n';
    NonTerminal nt;
    std::cout << nt << '\n';
    NonTerminal nt2;
    std::cout << nt2 << '\n';
    Terminal t;
    std::cout << t << '\n';

    Expression* list[] = { new NonTerminal(), new Terminal() };

    SerialVisitor sv;
    list[0]->accept(sv);
    list[1]->accept(sv);

    return 0;
}