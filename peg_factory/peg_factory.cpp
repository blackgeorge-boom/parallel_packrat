//
// Created by blackgeorge on 4/22/19.
//

#include "peg_factory.h"

std::map<Expression*, bool> pht;

PEG* PEGFactory::from_tree(TreeNode *root)
{
    nt_map.clear();
    auto result_peg = construct_peg(root);
    result_peg->set_pht(pht);
    return result_peg;
}

PEG* PEGFactory::construct_peg(TreeNode *node)
{
    if (node->name() == "Grammar") {

        PEG* peg = new PEG;
        std::pair<NonTerminal*, Expression*>* p;

        for (auto child : node->get_children())
            if (child->name() == "Definition") {
                p = construct_rule(child);
                peg->push_rule(p->first, p->second);
            }
        return peg;
    }
    else if (node->children_num() > 0)
        for (auto child : node->get_children())
            if (child->name() == "Grammar")
                return construct_peg(child);

    return nullptr;
}

std::pair<NonTerminal*, Expression*>* PEGFactory::construct_rule(TreeNode* node)
{
    NonTerminal* nt = construct_identifier(node->get_ith(0));
    Expression* e = construct_expression(node->get_ith(2));

    if (!nt or !e)
        throw "Cannot construct PEG; malformed parse tree!";

    return new std::pair<NonTerminal*, Expression*>(nt, e);
}

NonTerminal* PEGFactory::construct_identifier(TreeNode *node)
{
    std::string name;

    for (auto child : node->get_children()) {
        if (child->name() == "IdentifierStart" or child->name() == "IdentifierRest") {
            name += child->get_ith(0)->name();
            //TODO: add checking
        }
    }

    auto it = nt_map.find(name);

    if (it != nt_map.end())
        return it->second;
    else {
        auto nt = new NonTerminal(name);
        nt_map.insert(std::pair<std::string, NonTerminal*>(name, nt));
        return nt;
    }
}

Expression* PEGFactory::construct_expression(TreeNode *node)
{
    if (node->children_num() > 1) {

        auto ce = new CompositeExpression('/');

        for (auto child : node->get_children())
            if (child->name() == "Sequence") {
                auto child_expr = construct_sequence(child);
                ce->push_expr(child_expr);
                pht[child_expr] = 1;
            }

        return ce;
    }
    else {
        return construct_sequence(node->get_ith(0));
    }
}

Expression* PEGFactory::construct_sequence(TreeNode *node)
{
    if (node->children_num() > 1) {

        auto ce = new CompositeExpression('\b');

        for (auto child : node->get_children())
            if (child->name() == "Prefix") {
                Expression* e = construct_preffix(child);
//                auto temp = dynamic_cast<CompositeExpression*>(e);
//                if (temp != nullptr && temp->op_name() == '\b') {
//                    for (auto x : temp->expr_list())
//                        ce->push_expr(x);
//                }
//                else
                ce->push_expr(e);
            }

        return ce;
    }
    else {
        return construct_preffix(node->get_ith(0));
    }
}

Expression* PEGFactory::construct_preffix(TreeNode *node)
{
    auto child = node->get_ith(0);

    if (node->children_num() > 1) {

        char op;

        if (child->name() == "And")
            op = '&';
        else
            op = '!';

        auto ce = new CompositeExpression(op);
        ce->push_expr(construct_suffix(node->get_ith(1)));
        return ce;
    }
    else {
        return construct_suffix(child);
    }
}

Expression* PEGFactory::construct_suffix(TreeNode *node)
{
    auto child = node->get_ith(0);

    if (node->children_num() > 1) {

        char op;
        auto second_child = node->get_ith(1);

        if (second_child->name() == "QuestionMark")
            op = '?';
        else if (second_child->name() == "Star")
            op = '*';
        else
            op = '+';

        auto ce = new CompositeExpression(op);
        ce->push_expr(construct_primary(child));
        return ce;
    }
    else {
        return construct_primary(child);
    }
}

Expression* PEGFactory::construct_primary(TreeNode *node)
{
    auto child = node->get_ith(0);

    if (child->name() == "Identifier")
        return construct_identifier(child);
    else if (child->name() == "OpenParen")
        return construct_expression(node->get_ith(1));
    else if (child->name() == "Literal")
        return construct_literal(child);
    else
        return new AnyChar();
}

Expression* PEGFactory::construct_literal(TreeNode* node)
{
    std::vector<Expression*> terminals;

    for (auto child : node->get_children())
        if (child->name() == "Character")
            terminals.push_back(construct_char(child));

    if (terminals.size() > 1)
        return new CompositeExpression('\b', terminals);
    else if (terminals.size() == 1)
        return terminals[0];
    else {
        std::cout << "Error while constructing PEG grammar: no Character node under Literal node.";
        return nullptr;
    }
}

Expression* PEGFactory::construct_char(TreeNode* node)
{
    auto child = node->get_ith(0);

    if (node->children_num() == 2 && child->name() == "\\") {
        auto second_child = node->get_ith(1);
        std::string ch = second_child->name();

        if (ch == "n")
            return new Terminal("\n");
        else if (ch == "r")
            return new Terminal("\r");
        else if (ch == "t")
            return new Terminal("\t");
        else if (ch == "\\")
            return new Terminal("\\");
        else if (ch == "\'")
            return new Terminal("\'");
        else if (ch == "\"")
            return new Terminal("\"");
        else if (ch == "UnicodeEscape")
            return construct_unicode(second_child);
        else {
            std::cout << "Error while constructing PEG grammar: Character node contains invalid escape.";
            return nullptr;
        }
    }
    else if (node->children_num() == 1) {
        return child->get_expr();
    }
    else {
        std::cout << "Error while constructing PEG grammar: invalid Character node: " << *child << "\n";
        return nullptr;
    }
}

Expression *PEGFactory::construct_unicode(TreeNode* node)
{
    std::string u_start("\\u");

    auto u_first = node->get_ith(1)->get_ith(0)->name();
    auto u_second = node->get_ith(2)->get_ith(0)->name();
    auto u_third = node->get_ith(3)->get_ith(0)->name();
    auto u_fourth = node->get_ith(4)->get_ith(0)->name();
    return new Terminal(u_start + u_first + u_second + u_third + u_fourth);
}
