#include <utility>

//
// Created by blackgeorge on 21/3/2019.
//

#ifndef PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
#define PARALLEL_PACKRAT_SIMPLE_PARALLEL_H

#include "../serial/serial_packrat.h"

class SimpleParallel: public SerialPackrat {
public:
    SimpleParallel(std::string input, const PEG& g);
    SimpleParallel(const char* input, const PEG& g);
    SimpleParallel(std::string input, const PEG& g, Cell** c);

    bool visit(PEG& peg) override;
};

class SimpleWorker: public SimpleParallel {
    int left;
    int right;
public:
    SimpleWorker(std::string input, const PEG& g, Cell** c, int l, int r);
    ~SimpleWorker() override = default;

    bool visit(NonTerminal& nt) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_SIMPLE_PARALLEL_H
