//
// Created by blackgeorge on 16/3/2019.
//

#ifndef PARALLEL_PACKRAT_GRAMMAR_H
#define PARALLEL_PACKRAT_GRAMMAR_H

#include <map>

#include "peg_elements.h"

class PEG {
    std::map<NonTerminal*, CompositeExpression*> r;
    NonTerminal* s;
public:
    PEG() = default;
    virtual ~PEG() = default;

    void push_rule(NonTerminal* nt, CompositeExpression* ce);
    void set_start(NonTerminal* nt) { s = nt; }

    virtual void accept(class PegVisitor& pegv) = 0;
};


#endif //PARALLEL_PACKRAT_GRAMMAR_H
