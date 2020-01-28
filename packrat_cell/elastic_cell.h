//
// Created by blackgeorge on 3/12/19.
//

#ifndef PARALLEL_PACKRAT_ELASTIC_CELL_H
#define PARALLEL_PACKRAT_ELASTIC_CELL_H

#include <ostream>
#include <mutex>

#include "cell.h"
#include "../syntax_tree/tree_node.h"

class ElasticCell: public Cell {
protected:
    long int key;
public:
    ElasticCell() : Cell(), key{-1} {}
    ~ElasticCell() = default;

    int get_key() { return key; }
    void set_key(long int k) { key = k; }
};

#endif //PARALLEL_PACKRAT_ELASTIC_CELL_H
