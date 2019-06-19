#include <utility>

//
// Created by blackgeorge on 21/3/2019.
//

#ifndef PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
#define PARALLEL_PACKRAT_SIMPLE_PARALLEL_H

#include "locked_cell.h"
#include "../serial/serial_packrat.h"

class SimpleParallel: public SerialPackrat {
protected:
    LockedCell** cells;
public:
    SimpleParallel(std::string input, PEG g);
    SimpleParallel(const char* input, PEG g);
    SimpleParallel(std::string input, PEG g, LockedCell** c);

    bool visit(PEG& peg) override;
};

class SimpleWorker: public SimpleParallel {
    int left;
    int right;
public:
    SimpleWorker(std::string input, PEG g, LockedCell** c, int l, int r);
    ~SimpleWorker() override = default;

    bool visit(NonTerminal& nt) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
