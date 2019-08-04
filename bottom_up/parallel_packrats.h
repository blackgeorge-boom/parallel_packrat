//
// Created by blackgeorge on 22/7/2019.
//

#ifndef PARALLEL_PACKRAT_PARALLEL_PACKRATS_H
#define PARALLEL_PACKRAT_PARALLEL_PACKRATS_H

#include "../serial/serial_packrat.h"

class ColumnParallel: public SerialPackrat {
    int thread_num;
    bool rec_split;
public:
    ColumnParallel(std::string input, const PEG& g, int n, bool recursive_split)
        : SerialPackrat{input, g}, thread_num{n}, rec_split{recursive_split} {}

    bool visit(PEG& peg) override;
};

class RowParallel: public SerialPackrat {
    int thread_num;
    bool rec_split;
public:
    RowParallel(std::string input, const PEG& g, int n, bool recursive_split)
            : SerialPackrat{input, g}, thread_num{n}, rec_split{recursive_split} {}

    bool visit(PEG& peg) override;
};

class BlockParallel: public SerialPackrat {
    int thread_num;
    int block_size;
    bool rec_split;
public:
    BlockParallel(std::string input, const PEG& g, int n, int s, bool recursive_split)
            : SerialPackrat{input, g}, thread_num{n}, block_size{s}, rec_split{recursive_split} {}

    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_PARALLEL_PACKRATS_H
