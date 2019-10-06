//
// Created by blackgeorge on 4/12/19.
//

#ifndef PARALLEL_PACKRAT_PEG_H
#define PARALLEL_PACKRAT_PEG_H

#include <utility>

#include "peg_elements.h"

class PEG {
    std::map<NonTerminal*, Expression*> r;
    std::map<int, NonTerminal*> idx;
    std::map<Expression*, bool> pht;
    NonTerminal* s;
public:
    PEG(): r{}, idx{}, pht{}, s{} {};
    PEG(const PEG& peg);
    virtual ~PEG() = default;

    std::map<NonTerminal*, Expression*> get_rules() const { return r; }
    void push_rule(NonTerminal* nt, Expression* e);
    Expression* get_expr(NonTerminal* nt);

    std::map<int, NonTerminal*> get_index() const { return idx; }
    NonTerminal* get_non_term(int i);

    std::map<Expression*, bool> get_pht() { return pht; }
    void set_pht(std::map<Expression*, bool> m) { pht = std::move(m); }
    bool get_history(Expression* e);

    NonTerminal* get_start() const { return s; }
    void set_start(NonTerminal* nt) { s = nt; }

    bool accept(class PegVisitor& pegv);
};

std::ostream& operator<<(std::ostream& os, const PEG& peg);

#endif //PARALLEL_PACKRAT_PEG_H
