//
// Created by blackgeorge on 10/3/19.
//

#include "hash_serial.h"

#include <utility>

HashPackrat::HashPackrat(std::string input, const PEG &g)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);
    cells = nullptr;
}

bool HashPackrat::visit(NonTerminal& nt)
{
    int row = nt.index();
    auto coords = std::string(std::to_string(row) + "," + std::to_string(pos));

    auto it = memoCells.find(coords);
    if (it != memoCells.end()) {

        int cur_res = it->second;

        if (cur_res > 0) {
            pos = cur_res;
            return true;
        }
        return false;

    }
    else {
        Expression* e = peg.get_expr(&nt);
        auto res = e->accept(*this);
        int* cur_res = &memoCells[coords];

        if (res) {
            *cur_res = pos; // pos has changed
            return true;
        }
        else {
            *cur_res = -1;
            return false;
        }
    }
}

bool HashPackrat::visit(PEG& peg)
{
    std::cout << "Parsing..." << std::endl;
    NonTerminal* nt;
    bool res;

    nt = peg.get_start();
    return nt->accept(*this);

//    res = memoCells[std::string("0,0")] >= 0;

//    return res;
}
