//
// Created by blackgeorge on 3/12/19.
//

#ifndef PARALLEL_PACKRAT_CELL_H
#define PARALLEL_PACKRAT_CELL_H

#include <ostream>

#include "tree_node.h"

// you have constructor, assignment and comparison for those
enum class Result { unknown, fail, success };

class Cell {
    Result r;
    int p;
    TreeNode* node;
public:


public:
    Cell() :r{Result::unknown}, p{-1}, node{} {}
    ~Cell() = default;

    Result res() const { return r; }
    void set_res(Result result) { r = result; }
    int pos() const { return p; }
    void set_pos(int position) { p = position; }
    TreeNode* get_node() const { return node; }
    void set_node(TreeNode *tree_node) { node = tree_node; }
};

std::ostream& operator<<(std::ostream& os, const Cell& c);

#endif //PARALLEL_PACKRAT_CELL_H
