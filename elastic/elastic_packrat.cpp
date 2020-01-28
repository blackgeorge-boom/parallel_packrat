//
// Created by blackgeorge on 1/27/20.
//

#include "elastic_packrat.h"

#include <utility>
#include <cmath>

Elastic::Elastic(std::string input, const PEG &g, int window_size)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);
    w = window_size;
    n = peg.get_rules().size();

    shift = ceil(log2(n));

    std::cout << "Array size: " << n * w << "\n";
    elastic_cells = new ElasticCell[n * w];
}

bool Elastic::visit(NonTerminal& nt)
{
    int row = nt.index();
    long int key = (pos << shift) | row;
//    std::cout << "Key: " << key << std::endl;
//    unsigned int index = key % (w * n);
    unsigned int index = hash(key) % (w * n);
//    std::cout << "Index: " << index << std::endl;

    ElasticCell* cur_cell = &elastic_cells[index];
    Result cur_res = cur_cell->res();

    if (cur_cell->get_key() != key) {
        cur_res = Result::unknown;
        std::cout << "conflict\n";
    }

    switch (cur_res) {

        case Result::success:
        {
            pos = cur_cell->pos();
            return true;
        }
        case Result::fail:
        {
            return false;
        }
        case Result::unknown:
        {
            Expression* e = peg.get_expr(&nt);
            auto res = e->accept(*this);

            cur_cell->set_key(key);
            if (res) {
                cur_cell->set_res(Result::success);
                cur_cell->set_pos(pos); // pos has changed
                return true;
            } else {
                cur_cell->set_res(Result::fail);
                return false;
            }
        }
        case Result::pending:
        {
            std::cout << "Pending!?!" << std::endl;
            break;
        }
    }
    return false;
}

bool Elastic::visit(PEG& peg)
{
    std::cout << "Parsing..." << std::endl;
    NonTerminal* nt;

    nt = peg.get_start();
    return nt->accept(*this);
}

unsigned int hash(unsigned int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}
