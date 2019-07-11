//
// Created by blackgeorge on 11/7/2019.
//

#include <thread>
#include "simple_worker.h"

SimpleWorker::SimpleWorker(std::string input, const PEG &g, Cell **c, int l, int r, int b, int u)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);
    cells = c;
    
    left = l;
    right = r;
    bottom = b;
    up = u;
}

bool SimpleWorker::visit(NonTerminal &nt) 
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
        case Result::pending:
        {
            while (cur_cell->res() == Result::pending) {
//                std::cout << "Lets see..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            if (cur_cell->res() == Result::pending)
                std::cout << "wuuut" << std::endl;
            return cur_cell->res() == Result::success;
        }
        case Result::unknown:
        {
            cur_cell->lock();
            cur_cell->set_res(Result::pending);
            cur_cell->unlock();
            Expression* e = peg.get_expr(&nt);
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
    }
}

bool SimpleWorker::visit(PEG& peg)
{
    NonTerminal* nt;

    int N = peg.get_rules().size();

    for (int j = right - 1; j >= left; --j) {
        for (auto i = N - 1; i >= 0; --i) {
            pos = j;
            nt = peg.get_non_term(i);
            nt->accept(*this);
        }
    }
    return true;
}
