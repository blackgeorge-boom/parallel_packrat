//
// Created by blackgeorge on 18/3/20.
//

#ifndef PARALLEL_PACKRAT_CONC_ELASTIC_BASE_H
#define PARALLEL_PACKRAT_CONC_ELASTIC_BASE_H

#include "../packrat_cell/elastic_cell.h"
#include "../serial/serial_packrat.h"

#include "tbb/concurrent_hash_map.h"
#include "tbb/tbb_allocator.h"

// Structure that defines hashing and comparison operations for user's type.
struct MyHashCompare {
    static size_t hash(const long int x) {
        size_t h = 0;
        h = ((x >> 16) ^ x) * 0x45d9f3b;
        h = ((h >> 16) ^ h) * 0x45d9f3b;
        h = (h >> 16) ^ h;
        return h;
    }

    static bool equal(const long int x, const long int y) {
        return x==y;
    }
};

typedef tbb::concurrent_hash_map<long int, ElasticCell, MyHashCompare> ElasticTable;

// An abstract class for concurrent elastic packrat parsing.
class ConcurrentElasticBase: public SerialPackrat {
protected:
    int width, nt_num, shift;
    int thres;
    int* nt_elapsed;
    int* nt_utilized;
    int* nt_activated;
    ElasticTable* table;
public:
    void addData(const long int& key);

    void print_active() const;

    bool visit(NonTerminal& nt) override;
};

#endif //PARALLEL_PACKRAT_CONC_ELASTIC_BASE_H
