//
// Created by blackgeorge on 8/16/19.
//

#ifndef PARALLEL_PACKRAT_PARALLEL_PACKRATS_H
#define PARALLEL_PACKRAT_PARALLEL_PACKRATS_H

#include <utility>

#include "../serial/serial_packrat.h"

class TableParallel: public SerialPackrat {
    int expr_limit;
    int max_tree_depth;
public:
    TableParallel(std::string input, const PEG& g, int lim, int depth)
            : SerialPackrat{std::move(input), g}, expr_limit{lim}, max_tree_depth{depth} {}

    bool visit(CompositeExpression& ce) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_PARALLEL_PACKRATS_H
