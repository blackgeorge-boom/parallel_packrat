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

    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
