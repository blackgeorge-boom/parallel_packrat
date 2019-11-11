//
// Created by blackgeorge on 11/7/2019.
//

#ifndef PARALLEL_PACKRAT_SIMPLE_WORKER_H
#define PARALLEL_PACKRAT_SIMPLE_WORKER_H

#include "../serial/serial_packrat.h"
#include "stoppable.h"

class SimpleWorker: public SerialPackrat, public Stoppable{
public:
    SimpleWorker(std::string input, const PEG& g, Cell** c, int p);
    SimpleWorker(SimpleWorker&& sw) = default;
    ~SimpleWorker() override = default;

    bool visit(NonTerminal& nt) override;
};

#endif //PARALLEL_PACKRAT_SIMPLE_WORKER_H
