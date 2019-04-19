#include <utility>

//
// Created by blackgeorge on 4/19/19.
//

#ifndef PARALLEL_PACKRAT_TREE_NODE_H
#define PARALLEL_PACKRAT_TREE_NODE_H

#include "peg_elements.h"

class TreeNode {
    Expression* expr;
    std::vector<TreeNode*> children;
public:
    TreeNode() :expr(), children() {};
    explicit TreeNode(Expression* e) :expr(e), children() {};
    ~TreeNode() = default;

    Expression* get_expr() const { return expr; }
    std::vector<TreeNode*> get_children() const { return children; }
    void push_child(TreeNode* t) { children.push_back(t); }
    void push_children(std::vector<TreeNode*> v) { children = std::move(v); }
};

std::ostream& operator<<(std::ostream& os, const TreeNode& t);

void printTree(const std::string& prefix, const TreeNode* node, bool isLast);


#endif //PARALLEL_PACKRAT_TREE_NODE_H
