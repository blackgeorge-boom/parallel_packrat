//
// Created by blackgeorge on 11/7/2019.
//

#include "row_parallel.h"


#include <utility>
#include <thread>
#include <mutex>
#include <atomic>

#include "tbb/tbb.h"

#include "column_parallel.h"
#include "simple_worker.h"

RowParallel::RowParallel(const char* input, const PEG& g)
    : SerialPackrat(input, g) {}

RowParallel::RowParallel(std::string input, const PEG& g)
    : SerialPackrat(std::move(input), g) {}

std::mutex cout_mutex;  // for debugging with cout

bool RowParallel::visit(PEG& peg)
{
    std::cout << "Parsing..." << std::endl;

    // Almost Linear!
    tbb::task_scheduler_init tbb_init{4};

    int N = peg.get_rules().size();
    int M = in.size() + 1;
    std::cout << "N: " << N <<  std::endl;
    std::cout << "M: " << M <<  std::endl;

    std::atomic<int> monotonic_begin{N - 1};

    tbb::parallel_for(tbb::blocked_range<int>(0, N),
        [&](const tbb::blocked_range<int>& r)
        {
//            int r_size = static_cast<int>(r.size());        // !!!
//            int begin = monotonic_begin.fetch_sub(r_size);  // !!!
//            int end = begin + r_size;                       // !!!

            // TODO: Deadlocks!!
            for (int i = r.begin(); i < r.end(); ++i) {
                SimpleWorker sw(in, peg, cells, 0, M, i + 1, i);
                peg.accept(sw);
//                cout_mutex.lock();
//                std::cout << std::this_thread::get_id()<< ", " << i << std::endl;
//                print_cells();
//                cout_mutex.unlock();
            }
        }
    );

//    print_cells();
    auto res = cells[0][0].res() == Result::success;
    return res;
}
