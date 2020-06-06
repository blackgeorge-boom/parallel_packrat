//
// Created by blackgeorge on 4/12/19.
//

#include "peg_visitor.h"
#include "../peg/peg_elements.h"

bool NonTerminal::accept(PegVisitor& pegv)
{
    return pegv.visit(*this);
}

bool Terminal::accept(PegVisitor& pegv)
{
    return pegv.visit(*this);
}

bool CompositeExpression::accept(PegVisitor& pegv)
{
    return pegv.visit(*this);
}

bool Empty::accept(PegVisitor& pegv)
{
    return pegv.visit(*this);
}

bool AnyChar::accept(PegVisitor& pegv)
{
    return pegv.visit(*this);
}

bool PEG::accept(class PegVisitor& pegv)
{
    return pegv.visit(*this);
}

