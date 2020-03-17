//
// Created by blackgeorge on 1/27/20.
//

#ifndef PARALLEL_PACKRAT_ELASTIC_PACKRAT_H
#define PARALLEL_PACKRAT_ELASTIC_PACKRAT_H

#include <utility>

#include "../packrat_cell/elastic_cell.h"
#include "../packrat_cell/temp_cell.h"
#include "../serial/serial_packrat.h"

#include "tbb/concurrent_hash_map.h"
#include "tbb/tbb_allocator.h"

// Structure that defines hashing and comparison operations for user's type.
struct MyHashCompare {
    static size_t hash(const long unsigned int x) {
        size_t h = 0;
        h = ((x >> 16) ^ x) * 0x45d9f3b;
        h = ((h >> 16) ^ h) * 0x45d9f3b;
        h = (h >> 16) ^ h;
        return h;
    }

    static bool equal(const long unsigned int x, const long unsigned int y) {
        return x==y;
    }
};

typedef tbb::concurrent_hash_map<long unsigned int, TempCell, MyHashCompare> ElasticTable;

class Elastic: public SerialPackrat {
    int w;
    int n;
    int shift;
    int nt_lim;
    int* nt_elapsed;
    bool* nt_utilized;
    bool* nt_activated;
    ElasticCell* elastic_cells;
    ElasticTable table;

public:
    Elastic(std::string input, const PEG& g, int window_size, int threshold);

    void addData(const long unsigned int& key);

    void print_active() const;

    bool visit(NonTerminal& nt) override;
    bool visit(PEG& peg) override;
};

unsigned int hash(unsigned int x);

#endif //PARALLEL_PACKRAT_ELASTIC_PACKRAT_H
