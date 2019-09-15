#include <utility>
#include <thread>

//
// Created by blackgeorge on 21/3/2019.
//

#include "serial_packrat.h"

SerialPackrat::SerialPackrat(const char* input, const PEG& g)
{
    in = input;
    pos = 0;
    peg = PEG(g);

    auto N = peg.get_rules().size();
    auto M = in.size() + 1;

    cells = new Cell*[N];
    for(int i = 0; i < N; ++i)
        cells[i] = new Cell[M];
}

SerialPackrat::SerialPackrat(std::string input, const PEG& g)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);

    auto N = peg.get_rules().size();
    auto M = in.size() + 1;

    cells = new Cell*[N];
    for(int i = 0; i < N; ++i)
        cells[i] = new Cell[M];
}

void SerialPackrat::print_cells() const
{
    auto N = peg.get_rules().size();
    auto M = in.size() + 1;

    std::cout << std::endl;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < M; ++j)
            std::cout << cells[i][j] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool SerialPackrat::visit(NonTerminal& nt)
{
    int row = nt.index();
    Cell* cur_cell = &cells[row][pos];
    Result cur_res = cur_cell->res();

    switch (cur_res) {

        case Result::success:
        {
            pos = cur_cell->pos();
            return true;
        }
        case Result::fail:
        {
            return false;
        }
        case Result::unknown:
        {
            Expression *e = peg.get_expr(&nt);
            auto res = e->accept(*this);

            if (res) {
                cur_cell->set_res(Result::success);
                cur_cell->set_pos(pos); // pos has changed
                return true;
            } else {
                cur_cell->set_res(Result::fail);
                return false;
            }
        }
        case Result::pending:
        {
            std::cout << "Pending!?!" << std::endl;
            break;
        }
    }
    return false;
}

bool SerialPackrat::visit(Terminal &t)
{
    if (pos < in.size() && t.name()[0] == this->cur_tok()) {
        pos++;
        return true;
    }
    return false;
}

bool SerialPackrat::visit(CompositeExpression &ce)
{
    char op = ce.op_name();
    std::vector<Expression*> exprs = ce.expr_list();
    int orig_pos = pos;

    switch (op) {

        case '\b':  // sequence
        {
            for (auto expr : exprs)
                if (!expr->accept(*this)) {
                    pos = orig_pos;
                    return false;
                }
            return true;
        }
        case '/':   // ordered choice
        {
            for (auto expr : exprs) {
                pos = orig_pos;
                if (expr->accept(*this))
                    return true;
            }
            pos = orig_pos;
            return false;
        }
        case '&':   // followed by
        {
            auto res = exprs[0]->accept(*this);
            pos = orig_pos;
            return res;
        }
        case '!':   // not followed by
        {
            auto res = exprs[0]->accept(*this);
            pos = orig_pos;
            return !res;
        }
        case '?':   // optional
        {
            exprs[0]->accept(*this);
            return true;
        }
        case '*':   // repetition
        {
            while (exprs[0]->accept(*this)) ;
            return true;
        }
        case '+':   // positive repetition
        {
            if (!exprs[0]->accept(*this))
                return false;
            while (exprs[0]->accept(*this)) ;
            return true;
        }
        default:
        {
            std::cout << "Visiting not handled!";
            return false;
        }
    }
}

bool SerialPackrat::visit(Empty &e)
{
    return true;
}

bool SerialPackrat::visit(AnyChar &ac)
{
    if (pos < in.size()) {
        pos++;
        return true;
    }
    return false;
}

bool SerialPackrat::visit(PEG& p)
{
    std::cout << "Parsing..." << std::endl;
    NonTerminal* nt;
    bool res;

    nt = peg.get_start();
    nt->accept(*this);
    res = cells[0][0].res() == Result::success;
    return res;

    int N = peg.get_rules().size();
    int M = in.size() + 1;

    std::cout << "N: " << N << "\n";
    std::cout << "M: " << M << "\n";

    for (auto i = N - 1; i >= 0; --i) {
        for (auto j = M - 1; j >= 0; --j) {
            pos = j;
            nt = peg.get_non_term(i);
            nt->accept(*this);
        }
    }

    res = cells[0][0].res() == Result::success;
    return res;
}

