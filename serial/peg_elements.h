//
// Created by blackgeorge on 3/12/19.
//

#ifndef PARALLEL_PACKRAT_PEG_ELEMENTS_H
#define PARALLEL_PACKRAT_PEG_ELEMENTS_H

#include <string>
#include <iostream>
#include <vector>

class Expression {
    std::string n;
public:
    Expression() :n{} {};
    explicit Expression(std::string& name) :n{name} {};
    virtual ~Expression() = default;

    std::string name() const { return n; }

    virtual void accept(class PegVisitor& pegv) = 0;
};

std::ostream& operator<<(std::ostream& os, const Expression& e);

class NonTerminal: public Expression{
    static int num;
    int idx;
public:
    NonTerminal();
    explicit NonTerminal(std::string& name);
    ~NonTerminal() override = default;

    int index() const { return idx; }

    void accept(PegVisitor& pegv) override;
};

class Terminal: public Expression {
public:
    Terminal() : Expression() {};
    explicit Terminal(std::string& name) : Expression(name) {};
    ~Terminal() override = default;

    void accept(PegVisitor& pegv) override;
};

class CompositeExpression: public Expression {
    char op;
    std::vector<Expression*> expr;
public:
    CompositeExpression();
    CompositeExpression(std::string& name, char c);
    ~CompositeExpression() override = default;

    char op_name() const { return op; }
    void push_expr(Expression* e) { expr.push_back(e); }
    std::vector<Expression*> expr_list() const { return expr; };

    void accept(PegVisitor& pegv) override;
};

std::ostream& operator<<(std::ostream& os, const CompositeExpression& ce);

class PegVisitor {
public:
    virtual void visit(NonTerminal& nt) = 0;
    virtual void visit(Terminal& t) = 0;
    virtual void visit(CompositeExpression& ce) = 0;
};

class SerialVisitor: public PegVisitor {
public:
    void visit(NonTerminal& nt) override {
        std::cout << "Do up on " << nt << std::endl;
    }
    void visit(Terminal& t) override {
        std::cout << "Do up on " << t << std::endl;
    }
    void visit(CompositeExpression& ce) override {
        std::cout << "Do up on " << ce << std::endl;
    }
};

// TODO: rules are map<Nonterminal, expression>
// TODO: grammar has the map and the first rule as attributes
// TODO: expressions

#endif //PARALLEL_PACKRAT_PEG_ELEMENTS_H
