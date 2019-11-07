//
// Created by blackgeorge on 11/7/2019.
//

#include <mutex>
#include <thread>

#include "simple_worker.h"

SimpleWorker::SimpleWorker(std::string input, const PEG &g, Cell **c, int p)
{
    in = std::move(input);
    peg = PEG(g);
    cells = c;
    pos = p;
}

bool SimpleWorker::visit(NonTerminal &nt) 
{
    if (stopRequested())
        return false;

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
                std::cout << "Stuck: " << std::this_thread::get_id() <<
                " " << row << ", " << pos << " " << nt << std::endl;
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
