//
// Created by blackgeorge on 3/12/19.
//

#include <ostream>

#include "peg_elements.h"

std::ostream &operator<<(std::ostream &os, const Expression &e)
{
    return os << "\"" << e.name() << "\"";
}

std::ostream &operator<<(std::ostream &os, const CompositeExpression &ce)
{
    std::vector<Expression*> expressions = ce.expr_list();
    char op = ce.op_name();

    if (expressions.size() == 1) {
        if (op == '*')
            os << "(" << expressions[0]->name() << ")*";
        else
            os << op << "(" << expressions[0]->name() << ")";
    }
    else {
        for (auto x : expressions) {
            os << x->name() << " \'" << op << "\' ";
        }
        os << "\b\b\b\b";
    }
    return os;
}

NonTerminal::NonTerminal() : Expression()
{
    idx = num++;
}

NonTerminal::NonTerminal(const char* name) : Expression(name)
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

CompositeExpression::CompositeExpression(const char* name, char c) : Expression(name)
{
    op = c;
}

void CompositeExpression::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}
