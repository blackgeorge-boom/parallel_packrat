#include <utility>

//
// Created by blackgeorge on 4/19/19.
//

#ifndef PARALLEL_PACKRAT_TREE_NODE_H
#define PARALLEL_PACKRAT_TREE_NODE_H

#include "peg_elements.h"

class TreeNode {
    static int num;
    Expression* expr;
    std::vector<TreeNode*> children;
public:
    TreeNode() :children() { expr = new NonTerminal("anon_" + std::to_string(num++)); };
    explicit TreeNode(Expression* e) :expr(e), children() {};
    ~TreeNode() = default;

    Expression* get_expr() const { return expr; }
    std::vector<TreeNode*> get_children() const { return children; }
    TreeNode* get_ith(int i) { return children[i]; }
    std::string name() { return expr->name(); }
    long int children_num() const { return children.size(); }

    void push_child(TreeNode* t) { children.push_back(t); }
    void push_children(std::vector<TreeNode*> v) { children.insert(children.end(), v.begin(), v.end()); }
};

std::ostream& operator<<(std::ostream& os, const TreeNode& t);

void printTree(const std::string& prefix, const TreeNode* node, bool isLast);


#endif //PARALLEL_PACKRAT_TREE_NODE_H
