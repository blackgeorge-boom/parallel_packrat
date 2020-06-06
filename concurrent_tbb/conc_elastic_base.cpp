//
// Created by blackgeorge on 18/3/20.
//

#include <thread>
#include <iomanip>
#include "conc_elastic_base.h"

std::atomic<int> finished_rank;

void ConcurrentElasticBase::print_active()
{
    const int nt_width = 35;

    auto N = peg.get_rules().size();

    std::cout << std::endl;
    for(int i = 0; i < N; ++i) {
        std::cout << std::setw(nt_width) << *(peg.get_non_term(i)) << ": " << nt_activated[i] << " Elapsed: " << nt_elapsed[i] <<  "\n";
    }
    std::cout << std::endl;
}

unsigned int hash(unsigned int x) // TODO: further work
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}
