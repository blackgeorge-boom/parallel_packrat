//
// Created by blackgeorge on 4/12/19.
//

#include "peg.h"

PEG::PEG(const PEG &peg)
{
    r = peg.get_rules();
    s = peg.get_start();
}

void PEG::push_rule(NonTerminal *nt, CompositeExpression *ce)
{
    r.insert(std::pair<NonTerminal*, CompositeExpression*>(nt, ce));
    idx.insert(std::pair<int, NonTerminal*>(nt->index(), nt));
}

CompositeExpression* PEG::get_expr(NonTerminal* nt)
{
    return r.find(nt)->second;
}

NonTerminal* PEG::get_non_term(int i) {
    return idx.find(i)->second;
}

std::ostream &operator<<(std::ostream &os, const PEG &peg) {
    for (auto const& x : peg.get_rules()) {
        auto key = x.first;     // pointer to non terminal
        auto value = x.second;   // pointer to composite expression
        os << key->index() << ": " << *key << " -> " << *value << "\n";
    }
    return os;
}
