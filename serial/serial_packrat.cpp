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

bool SerialPackrat::visit(NonTerminal &nt)
{
    std::cout << "Visiting " << nt;
    int row = nt.index();
    Result cur_res = cells[row][pos].res();

    switch (cur_res) {

        case Result::success :
            pos = cells[row][pos].pos();
            return true;

        case Result::fail :
            return false;

        case Result::unknown:
            CompositeExpression* ce = peg.get_expr(&nt);
            auto res = ce->accept(*this);

            if (res) {
                cells
            }
    }
}

bool SerialPackrat::visit(Terminal &t)
{
    std::cout << "Visiting " << t;
    if (t.name()[0] == this->cur_tok()) {
        std::cout << "Parsed " << t;
        this->next_tok();
        return true;
    }
    return false;
}

bool SerialPackrat::visit(CompositeExpression &ce)
{
    for (auto x : ce.expr_list()) {
        std::cout << "x is : " << *x << "\n";
        x->accept(*this);
    }
}

bool SerialPackrat::visit(Empty &e)
{
    std::cout << "Visited " << e;
}

bool SerialPackrat::visit(AnyChar &ac)
{
    std::cout << "Visited " << ac;
}

bool SerialPackrat::visit(PEG &peg)
{
    std::cout << "Visiting peg \n";
    NonTerminal* nt = peg.get_start();
    std::cout << "Visited " << *nt;
    nt->accept(*this);
}

// TODO: when there is a syntax error?
