//
// Created by blackgeorge on 3/12/19.
//

#ifndef PARALLEL_PACKRAT_PEG_ELEMENTS_H
#define PARALLEL_PACKRAT_PEG_ELEMENTS_H

#include <string>
#include <iostream>
#include <vector>
#include <map>

class Expression {
    std::string n;
public:
    Expression() :n{} {};
    explicit Expression(const char* name) :n{name} {};
    virtual ~Expression() = default;

    std::string name() const { return n; }

    virtual std::ostream& put(std::ostream& os) const = 0;
    virtual bool accept(class PegVisitor& pegv) = 0;
};

std::ostream& operator<<(std::ostream& os, const Expression& e);

class NonTerminal: public Expression{
    static int num;
    int idx;
public:
    NonTerminal();
    explicit NonTerminal(const char* name);
    ~NonTerminal() override = default;

    int index() const { return idx; }

    const NonTerminal& operator=(const NonTerminal& nt);
    std::ostream& put(std::ostream& os) const override;
    bool accept(PegVisitor& pegv) override;
};

class Terminal: public Expression {
public:
    Terminal() : Expression() {};
    explicit Terminal(char c) : Expression(&c) {};
    explicit Terminal(const char* name) : Expression(name) {};
    ~Terminal() override = default;

    const Terminal& operator=(const Terminal& nt);
    std::ostream& put(std::ostream& os) const override;
    bool accept(PegVisitor& pegv) override;
};

class Empty: public Expression {
public:
    Empty() : Expression("epsilon") {};
    ~Empty() override = default;

    std::ostream& put(std::ostream& os) const override;
    bool accept(PegVisitor& pegv) override;
};

class AnyChar: public Expression {
public:
    AnyChar() : Expression(".") {};
    ~AnyChar() override = default;

    std::ostream& put(std::ostream& os) const override;
    bool accept(PegVisitor& pegv) override;
};

class CompositeExpression: public Expression {
    char op;
    std::vector<Expression*> expr;
public:
    CompositeExpression();
    explicit CompositeExpression(char c);
    CompositeExpression(char c, std::string&& v);
    ~CompositeExpression() override = default;

    char op_name() const { return op; }
    void push_expr(Expression* e) { expr.push_back(e); }
    std::vector<Expression*> expr_list() const { return expr; };

    const CompositeExpression& operator=(const CompositeExpression& ce);
    std::ostream& put(std::ostream& os) const override;
    bool accept(PegVisitor& pegv) override;
};

#endif //PARALLEL_PACKRAT_PEG_ELEMENTS_H
