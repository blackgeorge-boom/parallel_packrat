//
// Created by blackgeorge on 21/3/2019.
//

#include <utility>
#include <thread>
#include <mutex>

#include "tbb/tbb.h"

#include "simple_parallel.h"

std::mutex cout_mutex;

bool SimpleParallel::visit(PEG& peg)
{
    std::cout << "Parsing..." << std::endl;
    bool res;

    int M = in.size() + 1;

    std::cout << "M: " << M <<  std::endl;

    tbb::task_scheduler_init init(4);

    auto grain_size = 4;
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

    res = cells[0][0].res() == Result::success;
    return res;
}

SimpleWorker::SimpleWorker(std::string input, PEG g, Cell** c, int l, int r)
    : SerialPackrat(std::move(input), g, c)
{
    left = l;
    right = r;
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
