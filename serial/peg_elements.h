//
// Created by blackgeorge on 3/12/19.
//

#ifndef PARALLEL_PACKRAT_PEG_ELEMENTS_H
#define PARALLEL_PACKRAT_PEG_ELEMENTS_H

#include <string>
#include <iostream>
#include <vector>

class Expression {
public:
    virtual void operator<<(std::ostream& os) = 0;
    virtual void accept(class PegVisitor& pegv) = 0;
};

class NonTerminal: public Expression{
    std::string n;
    static int num;
    int idx;
public:
    NonTerminal() :n{}, idx{num++} {};
    ~NonTerminal() = default;
    std::string name() const { return n; }
    int index() const { return idx; }

    void accept(PegVisitor& pegv) override;
};

std::ostream& operator<<(std::ostream& os, const NonTerminal& nt);

class Terminal: public Expression {
    char c;
public:
    Terminal() :c{'0'} {};
    ~Terminal() = default;
    char name() const { return c; }

    void accept(PegVisitor& pegv) override;
};

std::ostream& operator<<(std::ostream& os, const Terminal& t);

class CompositeExpression: public Expression {
    char op;
    std::vector<Expression> expr;
public:
    CompositeExpression() :op{'*'}, expr{} {};
    ~CompositeExpression() = default;
    char op_name() const { return op; }
    std::vector<Expression> expr_list() const { return expr; };

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
