//
// Created by blackgeorge on 21/3/2019.
//

#include <utility>
#include <thread>
#include <mutex>

#include "tbb/tbb.h"

#include "simple_parallel.h"

SimpleParallel::SimpleParallel(const char* input, PEG g)
{
    in = input;
    pos = 0;
    peg = PEG(g);

    auto N = peg.get_rules().size();
    auto M = in.size() + 1;

    cells = new LockedCell*[N];
    for(int i = 0; i < N; ++i)
        cells[i] = new LockedCell[M];
}

SimpleParallel::SimpleParallel(std::string input, PEG g)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);

    auto N = peg.get_rules().size();
    auto M = in.size() + 1;

    cells = new LockedCell*[N];
    for(int i = 0; i < N; ++i)
        cells[i] = new LockedCell[M];
}

SimpleParallel::SimpleParallel(std::string input, PEG g, LockedCell** c)
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

    tbb::task_scheduler_init init(3);

    tbb::parallel_for(tbb::blocked_range<int>(0, M),
        [&](const tbb::blocked_range<int>& r)
        {
            for (int j = r.end() - 1; j >= r.begin(); --j) {
                SimpleWorker sw(in, peg, cells, j, j + 1);
                peg.accept(sw);
//                cout_mutex.lock();
//                std::cout << j << std::endl;
//                cout_mutex.unlock();
            }
        }
    );

    auto x = cells[0][0].res();
    res = cells[0][0].res() == Result::success;
    return res;
}

SimpleWorker::SimpleWorker(std::string input, PEG g, LockedCell** c, int l, int r)
    : SimpleParallel(std::move(input), g, c)
{
    left = l;
    right = r;
}

bool SimpleWorker::visit(NonTerminal& nt)
{
    int row = nt.index();
    LockedCell* cur_cell = &cells[row][pos];
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
        }
        case Result::unknown:
        {
            cur_cell->lock();
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
