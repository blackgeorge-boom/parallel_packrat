#include <iostream>

// you have constructor, assignment and comparison for those
enum class Result { unknown, fail, success };

class Cell {
    Result r;
    int p;
public:
    Cell() :r{Result::unknown}, p{-1} {};

    Result res() const { return r; }
    void res(Result result) { r = result; }
    int pos() const { return p; }
    void pos(int position) { p = position; }
};

std::ostream& operator<<(std::ostream& os, const Cell& c)
{
    return os << "{\"" << "\"," << c.pos() << "}";
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    Cell c;
    std::cout << c;
    return 0;
}