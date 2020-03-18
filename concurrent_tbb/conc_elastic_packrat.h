//
// Created by blackgeorge on 18/3/20.
//

#ifndef PARALLEL_PACKRAT_CONC_ELASTIC_PACKRAT_H
#define PARALLEL_PACKRAT_CONC_ELASTIC_PACKRAT_H

#include "conc_elastic_base.h"

class ConcurrentElasticPackrat: public ConcurrentElasticBase {
public:
    ConcurrentElasticPackrat(std::string input, const PEG& g, int window_size, int threshold);

    bool visit(CompositeExpression& ce) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_CONC_ELASTIC_PACKRAT_H
