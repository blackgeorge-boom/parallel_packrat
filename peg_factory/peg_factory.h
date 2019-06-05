//
// Created by blackgeorge on 4/22/19.
//

#ifndef PARALLEL_PACKRAT_PEG_FACTORY_H
#define PARALLEL_PACKRAT_PEG_FACTORY_H

#include "../syntax_tree/tree_node.h"
#include "../peg/peg_elements.h"
#include "../peg/peg.h"

class PEGFactory {
    std::map<std::string, NonTerminal*> nt_map;
public:
    PEGFactory(): nt_map() {}
    virtual ~PEGFactory() = default;

    PEG* from_tree(TreeNode* root);
    PEG* construct_peg(TreeNode* node);
    std::pair<NonTerminal*, Expression*>* construct_rule(TreeNode* node);
    NonTerminal* construct_identifier(TreeNode* node);
    Expression* construct_expression(TreeNode* node);
    Expression* construct_sequence(TreeNode* node);
    Expression* construct_preffix(TreeNode* node);
    Expression* construct_suffix(TreeNode* node);
    Expression* construct_primary(TreeNode* node);
    Expression* construct_literal(TreeNode* node);
    Expression* construct_char(TreeNode* node);
};
#endif //PARALLEL_PACKRAT_PEG_FACTORY_H
