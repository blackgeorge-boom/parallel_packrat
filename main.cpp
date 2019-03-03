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

class NonTerminal {
    std::string n;
    int idx;
public:
    NonTerminal() :n{}, idx{-1} {};
    ~NonTerminal() = default;
    std::string name() const { return n; }
    int index() const { return idx; }
};

std::ostream& operator<<(std::ostream& os, const NonTerminal& nt)
{
    return os << "{\"" << nt.name() << "\"," << nt.index() << "} \n";
}

class Terminal {
    char c;
public:
    Terminal() :c{'0'} {};
    ~Terminal() = default;
    char name() const { return c; }
};

std::ostream& operator<<(std::ostream& os, const Terminal& t)
{
    return os << "{\'" << t.name()  << "\'} \n";
}

class Element {
public:
    virtual void accept(class Visitor& v) = 0;
};

class This: public Element {
public:
    void accept(Visitor& v) override;
    std::string thiss() { return "This"; };
};

class That: public Element {
public:
    void accept(Visitor& v) override;
    std::string that() { return "That"; };
};

class Visitor {
public:
    virtual void visit(This* e) = 0;
    virtual void visit(That* e) = 0;
};

void This::accept(Visitor &v)
{
    v.visit(this);
}

void That::accept(Visitor &v)
{
    v.visit(this);
}

class UpVisitor: public Visitor {
public:
    void visit(This *e) override {
        std::cout << "Do up on " << e->thiss() << std::endl;
    }
    void visit(That *e) override {
        std::cout << "Do up on " << e->that() << std::endl;
    }
};

int main()
{
    std::cout << "Hello, World!" << std::endl;
    Cell c;
    std::cout << c;
    NonTerminal nt;
    std::cout << nt;
    Terminal t;
    std::cout << t;

    Element* list[] = { new This(), new That() };

    UpVisitor up;
    list[0]->accept(up);
    list[1]->accept(up);

    return 0;
}