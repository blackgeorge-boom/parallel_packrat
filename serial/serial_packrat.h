//
// Created by blackgeorge on 21/3/2019.
//

#ifndef PARALLEL_PACKRAT_SERIAL_PACKRAT_H
#define PARALLEL_PACKRAT_HASH_SERIAL_H

#include "../packrat_cell/cell.h"
#include "../peg_visitor/peg_visitor.h"

class SerialPackrat: public PegVisitor {
protected:
    std::string in;
    int pos;
    PEG peg;
    Cell** cells;
public:
    SerialPackrat() :in{}, pos{}, peg{}, cells{nullptr} {}
    SerialPackrat(const char* input, const PEG& g);
    SerialPackrat(std::string input, const PEG& g);
    ~SerialPackrat() override = default;
    // TODO: check more about destructors

    void print_cells() const;

    int cur_pos() { return pos; }
    char cur_tok() { return in[pos]; }
    void set_pos(int p) { pos = p; }

    bool visit(NonTerminal& nt) override;
    bool visit(Terminal& t) override;
    bool visit(CompositeExpression& ce) override;
    bool visit(Empty& e) override;
    bool visit(AnyChar& ac) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_SERIAL_PACKRAT_H
