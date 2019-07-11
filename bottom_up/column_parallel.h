#include <utility>

//
// Created by blackgeorge on 21/3/2019.
//

#ifndef PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
#define PARALLEL_PACKRAT_SIMPLE_PARALLEL_H

#include "../serial/serial_packrat.h"

class ColumnParallel: public SerialPackrat {
public:
    ColumnParallel(std::string input, const PEG& g);
    ColumnParallel(const char* input, const PEG& g);
    ColumnParallel(std::string input, const PEG& g, Cell** c);

    bool visit(PEG& peg) override;
};

class ColumnWorker: public ColumnParallel {
    int left;
    int right;
public:
    ColumnWorker(std::string input, const PEG& g, Cell** c, int l, int r);
    ~ColumnWorker() override = default;

    bool visit(NonTerminal& nt) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
