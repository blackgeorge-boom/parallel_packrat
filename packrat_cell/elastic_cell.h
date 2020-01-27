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
    int index;
public:
    ElasticCell() : Cell(), index{-1} {}
    ~ElasticCell() = default;
};

#endif //PARALLEL_PACKRAT_ELASTIC_CELL_H
