//
// Created by blackgeorge on 21/3/2019.
//

#include "serial_packrat.h"

SerialPackrat::SerialPackrat(const char* input, PEG g)
{
    in = input;
    pos = 0;
    peg = PEG(g);

    auto N = peg.get_rules().size();
    auto M = in.size();

    cells = new Cell*[N];
    for(int i = 0; i < N; ++i)
        cells[i] = new Cell[M];
}

void SerialPackrat::print_cells() const
{
    auto N = peg.get_rules().size();
    auto M = in.size();

    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < M; ++j)
            std::cout << cells[i][j] << " ";
        std::cout << "\n";
    }
}

void SerialPackrat::visit(NonTerminal &nt)
{
    std::cout << "Visiting nonterminal \n";
    CompositeExpression* ce = peg.get_expr(&nt);
    std::cout << "Visited " << *ce << "\n";
    ce->accept(*this);
}

void SerialPackrat::visit(Terminal &t)
{
    std::cout << "Visiting terminal \n";
    std::cout << "Visited " << t;
}

void SerialPackrat::visit(CompositeExpression &ce)
{
    std::cout << "Hello niggers \n";
    for (auto x : ce.expr_list()) {
        std::cout << "x is : " << *x << "\n";
        x->accept(*this);
    }
}

void SerialPackrat::visit(Empty &e)
{
    std::cout << "Visiting empty \n";
    std::cout << "Visited " << e;
}

void SerialPackrat::visit(AnyChar &ac)
{
    std::cout << "Visiting any char \n";
    std::cout << "Visited " << ac;
}

void SerialPackrat::visit(PEG &peg)
{
    std::cout << "Visiting peg \n";
    NonTerminal* nt = peg.get_start();
    std::cout << "Visited " << *nt;
    nt->accept(*this);
}

