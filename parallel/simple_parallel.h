#include <utility>

//
// Created by blackgeorge on 21/3/2019.
//

#ifndef PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
#define PARALLEL_PACKRAT_SIMPLE_PARALLEL_H

#include "../packrat_cell/cell.h"
#include "../serial/serial_packrat.h"

class SimpleParallel: public SerialPackrat {
public:
    SimpleParallel(std::string input, PEG g) : SerialPackrat(std::move(input), g) {}
    SimpleParallel(const char* input, PEG g) : SerialPackrat(input, g) {}

    bool visit(PEG& peg) override;
};

class SimpleWorker: public SerialPackrat {
    int left;
    int right;
public:
    SimpleWorker(std::string input, PEG g, Cell** c, int l, int r);
    ~SimpleWorker() override = default;

    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
