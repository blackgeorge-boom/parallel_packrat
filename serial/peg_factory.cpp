//
// Created by blackgeorge on 4/22/19.
//

#include "peg_factory.h"

PEG* PEGFactory::from_tree(TreeNode *root)
{
    nt_map.clear();
    return construct_peg(root);
}

PEG* PEGFactory::construct_peg(TreeNode *node)
{
    PEG* peg = new PEG;
    std::pair<NonTerminal*, CompositeExpression*>* p;

    if (node->name() == "Grammar") {
        for (auto child : node->get_children()) {
            p = construct_rule(child);
            peg->push_rule(p->first, p->second);
        }
        return peg;
    }
    else if (! node->get_children().empty()) {
        for (auto child : node->get_children())
            if (child->name() == "Grammar")
                return construct_peg(child);
    }

    return nullptr;
}

std::pair<NonTerminal*, CompositeExpression*>* PEGFactory::construct_rule(TreeNode *node)
{
    NonTerminal* nt = construct_identifier(node->get_children()[0]);
    CompositeExpression* ce = construct_expression(node->get_children()[2]);

    if (!nt or !ce)
        throw "Cannot construct PEG; malformed parse tree!";

    return new std::pair<NonTerminal*, CompositeExpression*>(nt, ce);
}

NonTerminal *PEGFactory::construct_identifier(TreeNode *node)
{
    std::string name;

    for (auto child : node->get_children()) {
        if (child->name() == "IdentifierStart" or child->name() == "IdentifierRest") {
            //TODO: add checking
            name += child->name();
        }
    }

    auto it = nt_map.find(name);

    if (it != nt_map.end())
        return it->second;
    else {
        NonTerminal* nt = new NonTerminal(name);
        nt_map.insert(std::pair<std::string, NonTerminal*>(name, nt));
        return nt;
    }
}
