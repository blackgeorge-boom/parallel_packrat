#include <utility>

//
// Created by blackgeorge on 21/3/2019.
//

#include <utility>
#include <thread>
#include <mutex>

#include "tbb/tbb.h"

#include "simple_parallel.h"

SimpleParallel::SimpleParallel(const char* input, const PEG& g)
    : SerialPackrat(input, g) {}

SimpleParallel::SimpleParallel(std::string input, const PEG& g)
    : SerialPackrat(std::move(input), g) {}

SimpleParallel::SimpleParallel(std::string input, const PEG& g, Cell** c)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);
    cells = c;
}

std::mutex cout_mutex;

bool SimpleParallel::visit(PEG& peg)
{
    std::cout << "Parsing..." << std::endl;
    bool res;

    int M = in.size() + 1;

    std::cout << "M: " << M <<  std::endl;

    tbb::task_scheduler_init init(4);

    tbb::parallel_for(tbb::blocked_range<int>(0, M),
        [&](const tbb::blocked_range<int>& r)
        {
            for (int j = r.end() - 1; j >= r.begin(); --j) {
//            for (int j = r.begin(); j < r.end(); ++j) {
                SimpleWorker sw(in, peg, cells, j, j + 1);
                peg.accept(sw);
//                cout_mutex.lock();
//                std::cout << std::this_thread::get_id()<< ", " << j << std::endl;
//                print_cells();
//                cout_mutex.unlock();
            }
        }
    );

//    print_cells();
    res = cells[0][0].res() == Result::success;
    return res;
}

SimpleWorker::SimpleWorker(std::string input, const PEG& g, Cell** c, int l, int r)
    : SimpleParallel(std::move(input), g, c)
{
    left = l;
    right = r;
}

bool SimpleWorker::visit(NonTerminal& nt)
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
            while (cur_cell->res() == Result::pending)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (cur_cell->res() == Result::pending)
                std::cout << "wuuut" << std::endl;
            return cur_cell->res() == Result::success;
        }
        case Result::unknown:
        {
            cur_cell->lock();
            Expression* e = peg.get_expr(&nt);
            auto res = e->accept(*this);

            if (res) {
                cur_cell->set_res(Result::success);
                cur_cell->set_pos(pos); // pos has changed
                cur_cell->unlock();
                return true;
            } else {
                cur_cell->set_res(Result::fail);
                cur_cell->unlock();
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
