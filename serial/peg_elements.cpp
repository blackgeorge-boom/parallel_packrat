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

NonTerminal::NonTerminal(std::string name) : Expression(name)
{
    idx = num++;
}

std::ostream &NonTerminal::put(std::ostream &os) const
{
    return os << this->name();
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

CompositeExpression::CompositeExpression(char c, std::string&& s)
{
    op = c;
    char aux[2];
    for (char x : s) {
        aux[0] = x;
        expr.push_back(new Terminal(aux));
    }
}

CompositeExpression::CompositeExpression(char c, std::initializer_list<Expression*> a_args)
        : op(c), expr(a_args) {}

std::ostream &CompositeExpression::put(std::ostream &os) const
{
    std::vector<Expression*> expressions = this->expr_list();
    char op = this->op_name();

    if (expressions.size() == 1) {
        if (op == '*' or op == '+' or op == '?')
            os << "(" << *expressions[0] << ")" << op;
        else
            os << op << "(" << *expressions[0] << ")";
    }
    else {
        for (auto x : expressions) {
            os << *x << ' ' << op << ' ';
        }
        os << "\b";     // eats last ' '
        if (op != '\b') os << "\b\b";   // eats last op and the ' ' before it
    }
    return os;
}




