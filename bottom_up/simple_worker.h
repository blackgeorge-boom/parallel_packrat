//
// Created by blackgeorge on 11/7/2019.
//

#ifndef PARALLEL_PACKRAT_SIMPLE_WORKER_H
#define PARALLEL_PACKRAT_SIMPLE_WORKER_H

#include "../serial/serial_packrat.h"

class SimpleWorker: public SerialPackrat {
    int left, right, bottom, up;
public:
    SimpleWorker(std::string input, const PEG& g, Cell** c, int l, int r, int b, int u);
    ~SimpleWorker() override = default;

    bool visit(NonTerminal& nt) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_SIMPLE_WORKER_H
