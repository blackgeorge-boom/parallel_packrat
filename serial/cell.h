//
// Created by blackgeorge on 3/12/19.
//

#ifndef PARALLEL_PACKRAT_CELL_H
#define PARALLEL_PACKRAT_CELL_H

#include <ostream>

// you have constructor, assignment and comparison for those
enum class Result { unknown, fail, success };

class Cell {
    Result r;
    int p;
public:
    Cell() :r{Result::unknown}, p{-1} {};
    ~Cell() = default;

    Result res() const { return r; }
    void res(Result result) { r = result; }
    int pos() const { return p; }
    void pos(int position) { p = position; }
};

std::ostream& operator<<(std::ostream& os, const Cell& c);

#endif //PARALLEL_PACKRAT_CELL_H
