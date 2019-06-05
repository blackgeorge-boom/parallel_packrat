//
// Created by blackgeorge on 4/12/19.
//

#ifndef PARALLEL_PACKRAT_PEG_VISITOR_H
#define PARALLEL_PACKRAT_PEG_VISITOR_H

#include "../peg/peg_elements.h"
#include "../peg/peg.h"

class PegVisitor {
public:
    virtual bool visit(NonTerminal& nt) = 0;
    virtual bool visit(Terminal& t) = 0;
    virtual bool visit(Empty& e) = 0;
    virtual bool visit(AnyChar& ac) = 0;
    virtual bool visit(CompositeExpression& ce) = 0;
    virtual bool visit(PEG& peg) = 0;
};

#endif //PARALLEL_PACKRAT_PEG_VISITOR_H
