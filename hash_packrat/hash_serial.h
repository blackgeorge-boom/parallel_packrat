//
// Created by blackgeorge on 10/3/19.
//

#ifndef PARALLEL_PACKRAT_HASH_SERIAL_H
#define PARALLEL_PACKRAT_HASH_SERIAL_H

#include <unordered_map>
#include "../serial/serial_packrat.h"

class HashPackrat: public SerialPackrat {
    std::unordered_map<int, int> memoCells{};
    int shift;
public:
    HashPackrat(std::string input, const PEG& g);
    ~HashPackrat() override = default;

    bool visit(NonTerminal& nt) override;
    bool visit(PEG& peg) override;
};

#endif //PARALLEL_PACKRAT_HASH_SERIAL_H
