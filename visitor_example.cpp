//
// Created by blackgeorge on 3/5/19.
//


#include <iostream>

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
    Element* list[] = { new This(), new That() };

    UpVisitor up;
    list[0]->accept(up);
    list[1]->accept(up);

    return 0;
};
