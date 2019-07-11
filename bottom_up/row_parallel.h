//
// Created by blackgeorge on 11/7/2019.
//

#ifndef PARALLEL_PACKRAT_ROW_PARALLEL_H
#define PARALLEL_PACKRAT_ROW_PARALLEL_H

#include "../serial/serial_packrat.h"

class RowParallel: public SerialPackrat {
public:
    RowParallel(std::string input, const PEG& g);
    RowParallel(const char* input, const PEG& g);
    RowParallel(std::string input, const PEG& g, Cell** c);

    bool visit(PEG& peg) override;
};

class RowWorker: public RowParallel {
    int bottom;
    int up;
public:
    RowWorker(std::string input, const PEG& g, Cell** c, int b, int u);
    ~RowWorker() override = default;

    bool visit(NonTerminal& nt) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_ROW_PARALLEL_H
