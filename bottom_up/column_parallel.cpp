
//
// Created by blackgeorge on 21/3/2019.
//

#include <utility>
#include <thread>
#include <mutex>
#include <atomic>

#include "tbb/tbb.h"

#include "column_parallel.h"
#include "simple_worker.h"

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

    int N = peg.get_rules().size();
    int M = in.size() + 1;
    std::cout << "N: " << M <<  std::endl;
    std::cout << "M: " << M <<  std::endl;

    std::atomic<int> monotonic_begin{M - 1};

    tbb::parallel_for(tbb::blocked_range<int>(0, M),
        [&](const tbb::blocked_range<int>& r)
        {
            int r_size = static_cast<int>(r.size());        // !!!
            int begin = monotonic_begin.fetch_sub(r_size);  // !!!
            int end = begin + r_size;                       // !!!

            for (int j = begin; j < end; ++j) {
                SimpleWorker sw(in, peg, cells, j, j + 1, N, 0);
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
