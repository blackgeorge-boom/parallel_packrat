//
// Created by blackgeorge on 11/7/2019.
//

#ifndef PARALLEL_PACKRAT_SIMPLE_WORKER_H
#define PARALLEL_PACKRAT_SIMPLE_WORKER_H

#include <atomic>

#include "../serial/serial_packrat.h"
#include "stoppable.h"

extern std::atomic<int> finished_rank;

class SimpleWorker: public SerialPackrat, public Stoppable {
    int expr_limit;
    int cur_tree_depth;
public:
    SimpleWorker(std::string input, const PEG& g, Cell** c, int p, int lim, int depth);
//    ~SimpleWorker() override {"destroy simple worker\n";}

    bool visit(CompositeExpression& ce) override;
    bool visit(NonTerminal& nt) override;
    bool visit(Terminal& t) override;
};

#endif //PARALLEL_PACKRAT_SIMPLE_WORKER_H
