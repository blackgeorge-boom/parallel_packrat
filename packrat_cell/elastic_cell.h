//
// Created by blackgeorge on 3/12/19.
//

#ifndef PARALLEL_PACKRAT_ELASTIC_CELL_H
#define PARALLEL_PACKRAT_ELASTIC_CELL_H

#include <ostream>
#include <mutex>
#include "cell.h"

#include "../syntax_tree/tree_node.h"

class ElasticCell {
protected:
    Result r;
    int p;
    long int key;
public:
    ElasticCell() :r{Result::unknown}, p{-1}, key{-1} {}
    ~ElasticCell() = default;

    Result res() const { return r; }
    void set_res(Result result) { r = result; }
    int pos() const { return p; }
    void set_pos(int position) { p = position; }
    int get_key() { return key; }
    void set_key(long int k) { key = k; }
};

#endif //PARALLEL_PACKRAT_ELASTIC_CELL_H
