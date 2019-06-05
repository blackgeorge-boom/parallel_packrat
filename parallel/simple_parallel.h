//
// Created by blackgeorge on 21/3/2019.
//

#ifndef PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
#define PARALLEL_PACKRAT_SIMPLE_PARALLEL_H

#include "../packrat_cell/cell.h"
#include "../serial/serial_packrat.h"

class SimpleParallel: public SerialPackrat {
public:
    SimpleParallel(const char* input, PEG g) : SerialPackrat(input, g) {}
    SimpleParallel(std::string input, PEG g) : SerialPackrat(input, g) {}
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
