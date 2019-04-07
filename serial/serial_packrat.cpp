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
    int row = nt.index();
    Cell* cur_cell = &cells[row][pos];
    Result cur_res = cur_cell->res();

    switch (cur_res) {

        case Result::success:
            pos = cur_cell->pos();
            return true;

        case Result::fail:
            return false;

        case Result::unknown:
            CompositeExpression* ce = peg.get_expr(&nt);
            auto res = ce->accept(*this);

            if (res) {
                cur_cell->set_res(Result::success);
                cur_cell->set_pos(pos); // pos has changed
                return true;
            }
            else {
                cur_cell->set_res(Result::fail);
                return false;
            }
    }
}

bool SerialPackrat::visit(Terminal &t)
{
    if (pos < in.size() && t.name()[0] == this->cur_tok()) {
        pos++;
//        std::cout << "Parsed " << t << "\n";
        return true;
    }
    return false;
}

bool SerialPackrat::visit(CompositeExpression &ce)
{
//    std::cout << "Visiting " << ce << "\n";
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
        case '!':   // not followed by
        {
            auto res = exprs[0]->accept(*this);
            pos = orig_pos;
            return !res;
        }
        default:
        {
            std::cout << "Something's wrong!";
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

bool SerialPackrat::visit(PEG &peg)
{
    std::cout << "Visiting peg \n";
    NonTerminal* nt = peg.get_start();
    auto res = nt->accept(*this);
    return res;
}

// TODO: when there is a syntax error?
