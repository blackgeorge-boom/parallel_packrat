//
// Created by blackgeorge on 11/7/2019.
//

#ifndef PARALLEL_PACKRAT_SIMPLE_WORKER_H
#define PARALLEL_PACKRAT_SIMPLE_WORKER_H

#include <atomic>

#include "../serial/serial_packrat.h"
#include "parallel_packrats.h"

class SimpleWorker: public SerialPackrat {
    int expr_limit;
    int cur_tree_depth;
    int max_tree_depth;
    int rank;
    std::atomic<int> finished_rank{-1};
    std::atomic<int>* parent_finished_rank;
public:
    SimpleWorker(std::string input, const PEG& g, Cell** c, int p, int lim, int cur_depth, int max_depth, int r, std::atomic<int>* pfr);

    bool visit(CompositeExpression& ce) override;
    bool visit(NonTerminal& nt) override;
    bool visit(Terminal& t) override;
};

#endif //PARALLEL_PACKRAT_SIMPLE_WORKER_H
