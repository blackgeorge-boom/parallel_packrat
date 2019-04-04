//
// Created by blackgeorge on 3/12/19.
//

#include <ostream>

#include "peg_elements.h"

std::ostream &operator<<(std::ostream &os, const Expression &e)
{
    return e.put(os);
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

std::ostream &NonTerminal::put(std::ostream &os) const
{
    return os << this->name();
}

void Terminal::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}

std::ostream &Terminal::put(std::ostream &os) const
{
    return os << this->name();
}

std::ostream &Empty::put(std::ostream &os) const
{
    return os << this->name();
}

std::ostream &AnyChar::put(std::ostream &os) const
{
    return os << this->name();
}

CompositeExpression::CompositeExpression() : Expression()
{
    op = '0';
}

CompositeExpression::CompositeExpression(char c) : Expression()
{
    op = c;
}

CompositeExpression::CompositeExpression(char c, std::vector<Expression*>&& v) : Expression()
{
    op = c;
    expr = v;
}

std::ostream &CompositeExpression::put(std::ostream &os) const
{
    std::vector<Expression*> expressions = this->expr_list();
    char op = this->op_name();

    if (expressions.size() == 1) {
        if (op == '*')
            os << *expressions[0] << "*";
        else
            os << op << *expressions[0];
    }
    else {
        for (auto x : expressions) {
            os << *x << ' ' << op << ' ';
        }
        os << "\b";
        if (op != '\b') os << "\b";
    }
    return os;
}

void CompositeExpression::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}

void Empty::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}

void AnyChar::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}

PEG::PEG(const PEG &peg)
{
    r = peg.get_rules();
    s = peg.get_start();
}

void PEG::push_rule(NonTerminal *nt, CompositeExpression *ce)
{
    r.insert(std::pair<NonTerminal*, CompositeExpression*>(nt, ce));
}

CompositeExpression* PEG::get_expr(NonTerminal* nt)
{
    return r.find(nt)->second;
}

void PEG::accept(class PegVisitor &pegv)
{
    pegv.visit(*this);
}

std::ostream &operator<<(std::ostream &os, const PEG &peg) {
    for (auto const& x : peg.get_rules()) {
        os << *x.first << " -> " << *x.second << "\n";
    }
    return os;
}

