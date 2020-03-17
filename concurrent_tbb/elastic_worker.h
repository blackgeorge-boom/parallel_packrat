//
// Created by blackgeorge on 11/7/2019.
//

#ifndef PARALLEL_PACKRAT_ELASTIC_WORKER_H
#define PARALLEL_PACKRAT_ELASTIC_WORKER_H

#include "../serial/serial_packrat.h"

class ElasticWorker: public SerialPackrat {
public:
    ElasticWorker(std::string input, const PEG& g, Cell** c, int p);

    bool visit(CompositeExpression& ce) override;
    bool visit(NonTerminal& nt) override;
    bool visit(Terminal& t) override;
};

#endif //PARALLEL_PACKRAT_ELASTIC_WORKER_H
