//
// Created by blackgeorge on 22/7/2019.
//

#include <utility>
#include <thread>
#include <mutex>
#include <atomic>

#include "tbb/tbb.h"

#include "parallel_packrats.h"
#include "simple_worker.h"

std::mutex cout_mutex;  // for debugging with cout

bool ColumnParallel::visit(PEG& peg)
{
    tbb::task_scheduler_init tbb_init{thread_num};

    int N = peg.get_rules().size();
    int M = in.size() + 1;
    std::cout << "N: " << N <<  std::endl;
    std::cout << "M: " << M <<  std::endl;

    if (rec_split) {
        tbb::parallel_for(tbb::blocked_range<int>(0, M),
            [&](const tbb::blocked_range<int>& r)
            {
                for (int j = r.begin(); j < r.end(); ++j) {
                    SimpleWorker sw(in, peg, cells, j, j + 1, 0, N);
                    peg.accept(sw);
                    cout_mutex.lock();
                    std::cout << std::this_thread::get_id()<< ", " << j << std::endl;
//                    print_cells();
                    cout_mutex.unlock();
                }
            }
        );
    }
    else {

        std::atomic<int> monotonic_begin{M - 1};

        tbb::parallel_for(tbb::blocked_range<int>(0, M),
            [&](const tbb::blocked_range<int>& r)
            {
                int r_size = static_cast<int>(r.size());        // !!!
                int begin = monotonic_begin.fetch_sub(r_size);  // !!!
                int end = begin + r_size;                       // !!!

                for (int j = begin; j < end; ++j) {
                    SimpleWorker sw(in, peg, cells, j, j + 1, 0, N);
                    peg.accept(sw);
                    cout_mutex.lock();
                    std::cout << std::this_thread::get_id()<< ", " << j << std::endl;
//                    print_cells();
                    cout_mutex.unlock();
                }
            }
        );
    }

    auto res = cells[0][0].res() == Result::success;
    return res;
}

bool RowParallel::visit(PEG& peg)
{
    // Almost Linear!
    tbb::task_scheduler_init tbb_init{thread_num};

    int N = peg.get_rules().size();
    int M = in.size() + 1;
    std::cout << "N: " << N <<  std::endl;
    std::cout << "M: " << M <<  std::endl;

    if (rec_split) {
        tbb::parallel_for(tbb::blocked_range<int>(0, N),
            [&](const tbb::blocked_range<int>& r)
            {
                for (int i = r.begin(); i < r.end(); ++i) {
                    SimpleWorker sw(in, peg, cells, 0, M, i, i + 1);
                    peg.accept(sw);
//                    cout_mutex.lock();
//                    std::cout << std::this_thread::get_id()<< ", " << i << std::endl;
//                    print_cells();
//                    cout_mutex.unlock();
                }
            }
        );
    }
    else {

        std::atomic<int> monotonic_begin{N - 1};

        tbb::parallel_for(tbb::blocked_range<int>(0, N),
            [&](const tbb::blocked_range<int>& r)
            {
                int r_size = static_cast<int>(r.size());        // !!!
                int begin = monotonic_begin.fetch_sub(r_size);  // !!!
                int end = begin + r_size;                       // !!!

                for (int i = begin; i < end; ++i) {
                    SimpleWorker sw(in, peg, cells, 0, M, i, i + 1);
                    peg.accept(sw);
//                    cout_mutex.lock();
//                    std::cout << std::this_thread::get_id()<< ", " << i << std::endl;
//                    print_cells();
//                    cout_mutex.unlock();
                }
            }
        );
    }

    auto res = cells[0][0].res() == Result::success;
    return res;
}


