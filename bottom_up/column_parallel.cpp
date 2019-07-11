
//
// Created by blackgeorge on 21/3/2019.
//

#include <utility>
#include <thread>
#include <mutex>
#include <atomic>

#include "tbb/tbb.h"

#include "column_parallel.h"
#include <utility>

ColumnParallel::ColumnParallel(const char* input, const PEG& g)
    : SerialPackrat(input, g) {}

ColumnParallel::ColumnParallel(std::string input, const PEG& g)
    : SerialPackrat(std::move(input), g) {}

ColumnParallel::ColumnParallel(std::string input, const PEG& g, Cell** c)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);
    cells = c;
}

std::mutex cout_mutex;  // for debugging with cout

bool ColumnParallel::visit(PEG& peg)
{
    std::cout << "Parsing..." << std::endl;

    int M = in.size() + 1;
    std::cout << "M: " << M <<  std::endl;

    std::atomic<int> monotonic_begin{M - 1};

    tbb::parallel_for(tbb::blocked_range<int>(0, M),
        [&](const tbb::blocked_range<int>& r)
        {
            int r_size = static_cast<int>(r.size());        // !!!
            int begin = monotonic_begin.fetch_sub(r_size);  // !!!
            int end = begin + r_size;                       // !!!

            for (int j = begin; j < end; ++j) {
                ColumnWorker sw(in, peg, cells, j, j + 1);
                peg.accept(sw);
//                cout_mutex.lock();
//                std::cout << std::this_thread::get_id()<< ", " << j << std::endl;
//                print_cells();
//                cout_mutex.unlock();
            }
        }
    );

//    print_cells();
    auto res = cells[0][0].res() == Result::success;
    return res;
}

ColumnWorker::ColumnWorker(std::string input, const PEG& g, Cell** c, int l, int r)
    : ColumnParallel(std::move(input), g, c)
{
    left = l;
    right = r;
}

bool ColumnWorker::visit(NonTerminal& nt)
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

bool ColumnWorker::visit(PEG& peg)
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
