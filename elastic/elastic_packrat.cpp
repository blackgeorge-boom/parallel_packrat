//
// Created by blackgeorge on 1/27/20.
//

#include "elastic_packrat.h"

#include <utility>
#include <cmath>

Elastic::Elastic(std::string input, const PEG &g, int window_size)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);
    w = window_size;
    shift = ceil(log2(peg.get_rules().size()));

    auto N = peg.get_rules().size();

    cells = new Cell*[N];
    for (int i = 0; i < N; ++i)
        cells[i] = new Cell[w];
}
