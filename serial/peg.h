//
// Created by blackgeorge on 4/12/19.
//

#ifndef PARALLEL_PACKRAT_PEG_H
#define PARALLEL_PACKRAT_PEG_H

#include "peg_elements.h"

class PEG {
    std::map<NonTerminal*, CompositeExpression*> r;
    std::map<int, NonTerminal*> idx;
    NonTerminal* s;
public:
    PEG(): r{}, idx{}, s{} {};
    PEG(const PEG& peg);
    virtual ~PEG() = default;

    void push_rule(NonTerminal* nt, CompositeExpression* ce);
    void set_start(NonTerminal* nt) { s = nt; }
    std::map<NonTerminal*, CompositeExpression*> get_rules() const { return r; }
    NonTerminal* get_start() const { return s; }

    CompositeExpression* get_expr(NonTerminal* nt);
    NonTerminal* get_non_term(int i);

    bool accept(class PegVisitor& pegv);
};

std::ostream& operator<<(std::ostream& os, const PEG& peg);

#endif //PARALLEL_PACKRAT_PEG_H
