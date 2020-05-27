//
// Created by blackgeorge on 8/16/19.
//

#ifndef PARALLEL_PACKRAT_PARALLEL_PACKRATS_H
#define PARALLEL_PACKRAT_PARALLEL_PACKRATS_H

#include <utility>

#include "../serial/serial_packrat.h"
#include "stoppable.h"

extern std::atomic<int> monotonic_begin;
extern std::atomic<int> flags[];

class TableParallel: public SerialPackrat {
    int expr_limit;
    int max_tree_depth;
    int flag_index;
public:
    TableParallel(std::string input, const PEG& g, int lim, int depth)
            : SerialPackrat{std::move(input), g}, expr_limit{lim}, max_tree_depth{depth}, flag_index{0} {}

    bool visit(CompositeExpression& ce) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_PARALLEL_PACKRATS_H
