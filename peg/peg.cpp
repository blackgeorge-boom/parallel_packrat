//
// Created by blackgeorge on 4/12/19.
//

#include "peg.h"

PEG::PEG(const PEG& peg)
{
    r = peg.get_rules();
    idx = peg.get_index();
    pht = peg.get_pht();
    s = peg.get_start();
    // TODO: fix memory leak
}

void PEG::push_rule(NonTerminal* nt, Expression* e)
{
    r.insert(std::pair<NonTerminal*, Expression*>(nt, e));
    idx.insert(std::pair<int, NonTerminal*>(nt->index(), nt));
}

Expression* PEG::get_expr(NonTerminal* nt)
{
    return r.find(nt)->second;
}

NonTerminal* PEG::get_non_term(int i)
{
    return idx.find(i)->second;
}

bool PEG::get_history(Expression *e)
{
    return pht[e];
}

std::ostream &operator<<(std::ostream &os, const PEG &peg)
{
    for (auto const& x : peg.get_rules()) {
        auto key = x.first;     // pointer to non terminal
        auto value = x.second;  // pointer to expression
        os << key->index() << ": " << *key << " -> " << *value << "\n";
    }
    return os;
}
