#include <iostream>

// you have constructor, assignment and comparison for those
enum class Result { unknown, fail, success };

class Cell {
    Result r;
    int p;
public:
    Cell() :r{Result::unknown}, p{-1} {};
    ~Cell() = default;

    Result res() const { return r; }
    void res(Result result) { r = result; }
    int pos() const { return p; }
    void pos(int position) { p = position; }
};

std::ostream& operator<<(std::ostream& os, const Cell& c)
{
    os << "{\"";
    switch (c.res()) {
        case Result::unknown:
            os << "UNKNOWN";
            break;
        case Result::fail:
            os << "FAIL";
            break;
        case Result::success:
            os << "SUCCESS";
            break;
        default:
            os << "Something's wrong";
    }
    return os << "\"," << c.pos() << "} \n";
}

class Expression {
public:
    virtual void accept(class PegVisitor& pegv) = 0;
};

class NonTerminal: public Expression{
    std::string n;
    int idx;
public:
    NonTerminal() :n{}, idx{-1} {};
    ~NonTerminal() = default;
    std::string name() const { return n; }
    int index() const { return idx; }

    void accept(PegVisitor& pegv) override;
};

std::ostream& operator<<(std::ostream& os, const NonTerminal& nt)
{
    return os << "{\"" << nt.name() << "\"," << nt.index() << "} \n";
}

class Terminal: public Expression {
    char c;
public:
    Terminal() :c{'0'} {};
    ~Terminal() = default;
    char name() const { return c; }

    void accept(PegVisitor& pegv) override;
};

std::ostream& operator<<(std::ostream& os, const Terminal& t)
{
    return os << "{\'" << t.name()  << "\'} \n";
}

class PegVisitor {
public:
    // why doesnt it work with &?
    virtual void visit(NonTerminal& nt) = 0;
    virtual void visit(Terminal& t) = 0;
};

void NonTerminal::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}

void Terminal::accept(PegVisitor &pegv)
{
    pegv.visit(*this);
}

class SerialVisitor: public PegVisitor {
public:
    void visit(NonTerminal& nt) override {
        std::cout << "Do up on " << nt << std::endl;
    }
    void visit(Terminal& t) override {
        std::cout << "Do up on " << t << std::endl;
    }
};

// TODO: rules are map<Nonterminal, expression>
// TODO: grammar has the map and the first rule as attributes
// TODO: expressions

int main()
{
    std::cout << "Hello, World!" << std::endl;
    Cell c;
    std::cout << c;
    NonTerminal nt;
    std::cout << nt;
    Terminal t;
    std::cout << t;

    Expression* list[] = { new NonTerminal(), new Terminal() };

    SerialVisitor sv;
    list[0]->accept(sv);
    list[1]->accept(sv);

    return 0;
}