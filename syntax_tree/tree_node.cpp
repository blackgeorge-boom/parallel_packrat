//
// Created by blackgeorge on 4/19/19.
//

#include "tree_node.h"

std::ostream& operator<<(std::ostream& os, const TreeNode& t)
{
    std::cout << *t.get_expr();
}

void printTree(const std::string& prefix, const TreeNode* node, bool isLast)
{
    if (node != nullptr)
    {
        std::cout << prefix;

        std::cout << (isLast ? "└──" : "├──" );

        // print the value of the node
        std::cout << *node << std::endl;

        auto children = node->get_children();

        if (children.empty()) return;

        for (auto i = 0; i < children.size() - 1; ++i)
            printTree(prefix + (isLast ? "    " : "│   "), children[i], false);

        printTree(prefix + (isLast ? "    " : "│   "), children.back(), true);
    }
}