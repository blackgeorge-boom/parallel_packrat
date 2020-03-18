//
// Created by blackgeorge on 18/3/20.
//

#include <cmath>

#include "conc_elastic_worker.h"

ConcurrentElasticWorker::ConcurrentElasticWorker(std::string input, int p, const PEG& g, int window_size, int threshold,
                                                 int* elapsed, int* utilized, int* activated, ElasticTable* et)
{
    in = std::move(input);
    pos = p;
    peg = PEG(g);

    width = window_size;
    nt_num = peg.get_rules().size();
    thres= threshold;

    shift = ceil(log2(nt_num));

    nt_elapsed = elapsed;
    nt_utilized = utilized;
    nt_activated = activated;

    table = et;
}
