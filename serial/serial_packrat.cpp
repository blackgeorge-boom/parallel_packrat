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
    CompositeExpression* ce = peg.get_expr(&nt);
    ce->accept(*this);
}

void SerialPackrat::visit(Terminal &t)
{
    std::cout << "Visited" << t;
}

void SerialPackrat::visit(CompositeExpression &ce)
{
    for (auto x : ce.expr_list())
        x->accept(*this);
}

void SerialPackrat::visit(Empty &e)
{
    std::cout << "Visited" << e;
}

void SerialPackrat::visit(AnyChar &ac)
{
    std::cout << "Visited" << ac;
}

void SerialPackrat::visit(PEG &peg)
{
    NonTerminal* nt = peg.get_start();
    nt->accept(*this);
}

