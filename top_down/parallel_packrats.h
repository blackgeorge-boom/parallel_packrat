//
// Created by blackgeorge on 8/16/19.
//

#ifndef PARALLEL_PACKRAT_PARALLEL_PACKRATS_H
#define PARALLEL_PACKRAT_PARALLEL_PACKRATS_H

#include "../serial/serial_packrat.h"
#include "stoppable.h"

class TableParallel: public SerialPackrat {
public:
    TableParallel(std::string input, const PEG& g)
            : SerialPackrat{input, g} {}

    bool visit(CompositeExpression& ce) override;
    bool visit(PEG& peg) override;
};

class MyTask: public Stoppable {
public:
    // Function to be executed by thread function
    void run(std::string in, );
};


#endif //PARALLEL_PACKRAT_PARALLEL_PACKRATS_H
