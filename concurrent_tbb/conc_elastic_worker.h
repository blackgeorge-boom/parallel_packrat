//
// Created by blackgeorge on 18/3/20.
//

#ifndef PARALLEL_PACKRAT_CONC_ELASTIC_WORKER_H
#define PARALLEL_PACKRAT_CONC_ELASTIC_WORKER_H

#include "conc_elastic_base.h"

class ConcurrentElasticWorker: public ConcurrentElasticBase {
public:
    ConcurrentElasticWorker(std::string input, int p, const PEG& g, int window_size, int threshold, int i,
                            int* elapsed, int* utilized, int* activated, ElasticCell* ec);

    bool visit(NonTerminal& nt) override;
    bool visit(CompositeExpression& ce) override;
};
#endif //PARALLEL_PACKRAT_CONC_ELASTIC_WORKER_H
