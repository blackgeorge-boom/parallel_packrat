#include <utility>

//
// Created by blackgeorge on 21/3/2019.
//

#include "tbb/tbb.h"

#include "simple_parallel.h"

bool SimpleParallel::visit(PEG &peg)
{
    std::cout << "\nParsing... \n";
    NonTerminal *nt;
    bool res;

    int N = peg.get_rules().size();
    int M = in.size() + 1;

    std::cout << "N: " << N << "\n";
    std::cout << "M: " << M << "\n";

    tbb::parallel_for(tbb::blocked_range<int>(0, M),
        [&](const tbb::blocked_range<int>& r)
        {
            for (int j = r.end() - 1; j >= r.begin(); ++j) {
                for (auto i = N - 1; i >= 0; --i) {
                    pos = j;
                    nt = peg.get_non_term(i);
                    nt->accept(*this);
                }
            }
        }
    );

    res = cells[0][0].res() == Result::success;
    return res;
}
