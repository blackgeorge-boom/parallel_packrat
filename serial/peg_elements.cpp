//
// Created by blackgeorge on 3/12/19.
//

#include <ostream>

#include "peg_elements.h"

std::ostream& operator<<(std::ostream& os, const NonTerminal& nt)
{
    return os << "{\"" << nt.name() << "\"," << nt.index() << "}";
}

std::ostream& operator<<(std::ostream& os, const Terminal& t)
{
    return os << "{\'" << t.name()  << "\'}";
}

std::ostream& operator<<(std::ostream& os, const CompositeExpression& ce)
{
    os << "{\'" << ce.op_name()  << "\'}";
    for (auto& i : ce.expr_list())
        os << i << "\n";
}

void NonTerminal::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}

void Terminal::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}

void CompositeExpression::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}
