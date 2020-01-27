//
// Created by blackgeorge on 1/27/20.
//

#ifndef PARALLEL_PACKRAT_ELASTIC_PACKRAT_H
#define PARALLEL_PACKRAT_ELASTIC_PACKRAT_H

#include <utility>

#include "../serial/serial_packrat.h"

class Elastic: public SerialPackrat {
    int w;
    int shift;
public:
    Elastic(std::string input, const PEG& g, int window_size);

    bool visit(NonTerminal& nt) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_ELASTIC_PACKRAT_H
