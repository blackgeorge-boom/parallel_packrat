//
// Created by blackgeorge on 1/27/20.
//

#ifndef PARALLEL_PACKRAT_ELASTIC_PACKRAT_H
#define PARALLEL_PACKRAT_ELASTIC_PACKRAT_H

#include <utility>

#include "../packrat_cell/elastic_cell.h"
#include "../serial/serial_packrat.h"

class Elastic: public SerialPackrat {
    int w;
    int n;
    int shift;
    char nt_lim;
    char* nt_elapsed;
    bool* nt_utilized;
    bool* nt_activated;
    ElasticCell* elastic_cells;
public:
    Elastic(std::string input, const PEG& g, int window_size, char threshold);

    bool visit(NonTerminal& nt) override;
    bool visit(PEG& peg) override;
};

unsigned int hash(unsigned int x);

#endif //PARALLEL_PACKRAT_ELASTIC_PACKRAT_H
