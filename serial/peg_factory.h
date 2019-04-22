//
// Created by blackgeorge on 4/22/19.
//

#ifndef PARALLEL_PACKRAT_PEG_FACTORY_H
#define PARALLEL_PACKRAT_PEG_FACTORY_H

#include "tree_node.h"
#include "peg_elements.h"
#include "peg.h"

class PEGFactory {
    std::map<std::string, NonTerminal*> nt_map;
public:
    PEGFactory(): nt_map() {}
    virtual ~PEGFactory() = default;

    PEG* from_tree(TreeNode* root);
    PEG* construct_peg(TreeNode* node);
    std::pair<NonTerminal*, CompositeExpression*>* construct_rule(TreeNode* node);
    NonTerminal* construct_identifier(TreeNode* node);
    CompositeExpression* construct_expression(TreeNode* node);
    CompositeExpression* construct_sequence(TreeNode* node);
    Expression* construct_preffix(TreeNode* node);
    Expression* construct_suffix(TreeNode* node);
    Expression* construct_primary(TreeNode* node);
    Expression* construct_literal(TreeNode* node);
    Terminal* construct_char(TreeNode* node);
};
#endif //PARALLEL_PACKRAT_PEG_FACTORY_H
