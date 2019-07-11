//
// Created by blackgeorge on 6/18/19.
//

#ifndef PARALLEL_PACKRAT_CELL_LOCKED_H
#define PARALLEL_PACKRAT_CELL_LOCKED_H


#include "../packrat_cell/cell.h"

class LockedCell : Cell {
    std::mutex m;
public:
    LockedCell() :Cell(), m{} {}
    ~LockedCell() = default;

    void lock() { m.lock(); }
    void unlock() { m.unlock(); }
};
#endif //PARALLEL_PACKRAT_CELL_LOCKED_H
