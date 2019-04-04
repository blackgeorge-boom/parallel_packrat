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
    virtual void accept(class PegVisitor& pegv) = 0;
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
    void accept(PegVisitor& pegv) override;
};

class Terminal: public Expression {
public:
    Terminal() : Expression() {};
    explicit Terminal(const char* name) : Expression(name) {};
    ~Terminal() override = default;

    const Terminal& operator=(const Terminal& nt);
    std::ostream& put(std::ostream& os) const override;
    void accept(PegVisitor& pegv) override;
};

class Empty: public Expression {
public:
    Empty() : Expression("epsilon") {};
    ~Empty() override = default;

    std::ostream& put(std::ostream& os) const override;
    void accept(PegVisitor& pegv) override;
};

class AnyChar: public Expression {
public:
    AnyChar() : Expression(".") {};
    ~AnyChar() override = default;

    std::ostream& put(std::ostream& os) const override;
    void accept(PegVisitor& pegv) override;
};

class CompositeExpression: public Expression {
    char op;
    std::vector<Expression*> expr;
public:
    CompositeExpression();
    explicit CompositeExpression(char c);
    explicit CompositeExpression(char c, std::vector<Expression*>&& v);
    ~CompositeExpression() override = default;

    char op_name() const { return op; }
    void push_expr(Expression* e) { expr.push_back(e); }
    std::vector<Expression*> expr_list() const { return expr; };

    const CompositeExpression& operator=(const CompositeExpression& ce);
    std::ostream& put(std::ostream& os) const override;
    void accept(PegVisitor& pegv) override;
};

class PEG {
    std::map<NonTerminal*, CompositeExpression*> r;
    NonTerminal* s;
public:
    PEG(): r{}, s{} {};
    PEG(const PEG& peg);
    virtual ~PEG() = default;

    void push_rule(NonTerminal* nt, CompositeExpression* ce);
    void set_start(NonTerminal* nt) { s = nt; }
    std::map<NonTerminal*, CompositeExpression*> get_rules() const { return r; }
    NonTerminal* get_start() const { return s; }

    CompositeExpression* get_expr(NonTerminal* nt);
    void accept(class PegVisitor& pegv);
};

std::ostream& operator<<(std::ostream& os, const PEG& peg);

class PegVisitor {
public:
    virtual void visit(NonTerminal& nt) = 0;
    virtual void visit(Terminal& t) = 0;
    virtual void visit(Empty& e) = 0;
    virtual void visit(AnyChar& ac) = 0;
    virtual void visit(CompositeExpression& ce) = 0;
    virtual void visit(PEG& peg) = 0;
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
    void visit(Empty& e) override {
        std::cout << "Do up on " << e << std::endl;
    }
    void visit(AnyChar& ac) override {
        std::cout << "Do up on " << ac << std::endl;
    }
    void visit(PEG& peg) override {
        std::cout << "Do up on " << peg << std::endl;
    }
};

#endif //PARALLEL_PACKRAT_PEG_ELEMENTS_H
