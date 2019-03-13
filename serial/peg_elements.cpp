//
// Created by blackgeorge on 3/12/19.
//

#include <ostream>

#include "peg_elements.h"

std::ostream &operator<<(std::ostream &os, const Expression &e) {
    return os << "\"" << e.name() << "\"";
}

std::ostream &operator<<(std::ostream &os, const CompositeExpression &ce) {
    os << "\"" << ce.op_name() << "\" \n";
    for (auto x : ce.expr_list()) {
        os << x->name() << " ";
    }
    os << "\n";
}

NonTerminal::NonTerminal() : Expression()
{
    idx = num++;
}

NonTerminal::NonTerminal(std::string &name) : Expression(name)
{
    idx = num++;
}

void NonTerminal::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}

void Terminal::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}

CompositeExpression::CompositeExpression() : Expression()
{
    op = '0';
}

CompositeExpression::CompositeExpression(std::string &name, char c) : Expression(name)
{
    op = c;
}

void CompositeExpression::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}
