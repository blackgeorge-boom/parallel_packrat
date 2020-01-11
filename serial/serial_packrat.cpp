#include <utility>
#include <thread>
#include <cstring>
#include <cmath>

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
    for (int i = 0; i < N; ++i)
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
    for (int i = 0; i < N; ++i)
        cells[i] = new Cell[M];
}

SerialPackrat::SerialPackrat(SerialPackrat&& sp) noexcept
{
    std::cout << "Move\n";
    in = sp.in; // TODO: check
    pos = sp.pos;
    peg = PEG(sp.peg); // TODO: check
    cells = sp.cells;
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

int hex2dec(std::string hex)
{
    int hex_digit = 0;
    int result = 0;

    auto len = hex.length();

    for (auto i = 0; i < len; ++i)
    {
        if(hex[i] >= '0' && hex[i] <= '9')
            hex_digit = hex[i] - 48;
        else if(hex[i] >= 'a' && hex[i] <= 'f')
            hex_digit = hex[i] - 87;
        else if(hex[i] >= 'A' && hex[i] <= 'F')
            hex_digit = hex[i] - 55;

        result += hex_digit * pow(16, len - i - 1);
    }

    return result;
}

bool SerialPackrat::visit(Terminal &t)
{
    int terminal_char = t.name()[0];

    if (t.name().length() > 1)
        terminal_char = hex2dec(t.name().substr(2, 4));

    if (pos < in.size() && terminal_char == this->cur_tok()) {
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

