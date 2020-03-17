//
// Created by blackgeorge on 3/12/19.
//

#ifndef PARALLEL_PACKRAT_TEMP_CELL_H
#define PARALLEL_PACKRAT_TEMP_CELL_H

#include <ostream>
#include <mutex>
#include "cell.h"

#include "../syntax_tree/tree_node.h"


class TempCell {
protected:
    Result r;
    int p;
public:
    TempCell() :r{Result::unknown}, p{-1} {}
    ~TempCell() = default;

    Result res() const { return r; }
    void set_res(Result result) { r = result; }
    int pos() const { return p; }
    void set_pos(int position) { p = position; }
};

#endif //PARALLEL_PACKRAT_TEMP_CELL_H
