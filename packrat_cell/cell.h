//
// Created by blackgeorge on 3/12/19.
//

#ifndef PARALLEL_PACKRAT_CELL_H
#define PARALLEL_PACKRAT_CELL_H

#include <ostream>
#include <mutex>

#include "../syntax_tree/tree_node.h"

// you have constructor, assignment and comparison for those
enum class Result { unknown, fail, success, pending };

class Cell {
protected:
    Result r;
    int p;
    TreeNode* node;
    std::mutex m;
public:
    Cell() :r{Result::unknown}, p{-1}, node{} {}
    ~Cell() = default;

    Result res() const { return r; }
    void set_res(Result result) { r = result; }
    int pos() const { return p; }
    void set_pos(int position) { p = position; }
    TreeNode* get_node() const { return node; }
    void set_node(TreeNode *tree_node) { node = tree_node; }

    void lock() { m.lock(); }
    void unlock() { m.unlock(); }
};

std::ostream& operator<<(std::ostream& os, const Cell& c);

#endif //PARALLEL_PACKRAT_CELL_H
