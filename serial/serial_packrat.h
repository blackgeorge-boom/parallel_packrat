//
// Created by blackgeorge on 21/3/2019.
//

#ifndef PARALLEL_PACKRAT_SERIAL_PACKRAT_H
#define PARALLEL_PACKRAT_SERIAL_PACKRAT_H

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

    SerialPackrat(SerialPackrat& sp) = delete;
    SerialPackrat(SerialPackrat&& sp) noexcept;
    SerialPackrat& operator=(SerialPackrat& sp) = delete;
    SerialPackrat&& operator=(SerialPackrat&& sp) = delete;
//    ~SerialPackrat() override = default;
    ~SerialPackrat() override {"Destroying Serial Packrat";}

    void print_cells() const;

    int cur_pos() { return pos; }
    char cur_tok() { return in[pos]; }

    bool visit(NonTerminal& nt) override;
    bool visit(Terminal& t) override;
    bool visit(CompositeExpression& ce) override;
    bool visit(Empty& e) override;
    bool visit(AnyChar& ac) override;
    bool visit(PEG& peg) override;
};

int hex2dec(std::string hex);

#endif //PARALLEL_PACKRAT_SERIAL_PACKRAT_H
