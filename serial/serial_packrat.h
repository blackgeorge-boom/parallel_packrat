//
// Created by blackgeorge on 21/3/2019.
//

#ifndef PARALLEL_PACKRAT_SERIAL_PACKRAT_H
#define PARALLEL_PACKRAT_SERIAL_PACKRAT_H

#include "cell.h"
#include "peg_elements.h"

class SerialPackrat: public PegVisitor {
    std::string in;
    int pos;
    PEG peg;
    Cell** cells;
public:
    SerialPackrat(const char* input, PEG g);

    void print_cells() const;

    void visit(NonTerminal& nt) override;
    void visit(Terminal& t) override;
    void visit(CompositeExpression& ce) override;
    void visit(Empty& e) override;
    void visit(AnyChar& ac) override;
    void visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_SERIAL_PACKRAT_H
