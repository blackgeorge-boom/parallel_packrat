//
// Created by blackgeorge on 18/3/20.
//

#ifndef PARALLEL_PACKRAT_CONC_ELASTIC_BASE_H
#define PARALLEL_PACKRAT_CONC_ELASTIC_BASE_H

#include <atomic>

#include "../packrat_cell/elastic_cell.h"
#include "../serial/serial_packrat.h"

extern std::atomic<int> finished_rank;

// An abstract class for concurrent elastic packrat parsing.
class ConcurrentElasticBase: public SerialPackrat {
protected:
    int width, nt_num, shift;
    int thres;
    int* nt_elapsed;
    int* nt_utilized;
    int* nt_activated;
    int rank;
    ElasticCell* elastic_cells;
public:
    void print_active();

    bool visit(NonTerminal& nt) override = 0;
};

unsigned int hash(unsigned int x);

#endif //PARALLEL_PACKRAT_CONC_ELASTIC_BASE_H
