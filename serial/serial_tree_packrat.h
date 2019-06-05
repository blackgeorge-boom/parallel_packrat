//
// Created by blackgeorge on 4/19/19.
//

#ifndef PARALLEL_PACKRAT_SERIAL_TREE_PACKRAT_H
#define PARALLEL_PACKRAT_SERIAL_TREE_PACKRAT_H

#include "../syntax_tree/tree_node.h"
#include "serial_packrat.h"

class SerialTreePackrat: public SerialPackrat {
    TreeNode* root;
    TreeNode* tree_pos;
public:
    SerialTreePackrat(const char* input, PEG g);
    SerialTreePackrat(std::string input, PEG g);

    TreeNode* get_root() { return root; }

    bool visit(NonTerminal& nt) override;
    bool visit(Terminal& t) override;
    bool visit(CompositeExpression& ce) override;
    bool visit(Empty& e) override;
    bool visit(AnyChar& ac) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_SERIAL_TREE_PACKRAT_H
