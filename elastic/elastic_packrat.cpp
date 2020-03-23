//
// Created by blackgeorge on 1/27/20.
//

#include "elastic_packrat.h"

#include <utility>
#include <cmath>

Elastic::Elastic(std::string input, const PEG &g, int window_size, int threshold)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);
    w = window_size;
    n = peg.get_rules().size();
    nt_lim = threshold;

    shift = ceil(log2(n));

    nt_elapsed = new int[n];
    nt_utilized = new bool[n];
    nt_activated = new bool[n];
    for (auto i = 0; i < n; ++i) {
        nt_elapsed[i] = nt_lim;
        nt_utilized[i] = false;
        nt_activated[i] = true;
    }

//    std::cout << "Array size: " << n * w << "\n";
    elastic_cells = new ElasticCell[n * w];
}

bool Elastic::visit(NonTerminal& nt)
{
    int row = nt.index();

    if (!nt_activated[row]) {
        Expression* e = peg.get_expr(&nt);
        return e->accept(*this);
    }

    if (nt_elapsed[row] >= 0)
        nt_elapsed[row] = nt_elapsed[row] - 1;

    long int key = (pos << shift) | row;
//    unsigned int index = key % (w * n);   // TODO
    unsigned int index = hash(key) % (w * n);

    ElasticCell* cur_cell = &elastic_cells[index];
    Result cur_res = cur_cell->res();

    if (cur_cell->get_key() != key) {
        cur_res = Result::unknown;
//        std::cout << "conflict\n";
    }
//    else
//        std::cout << "no conflict\n";   // TODO: benchmark

    switch (cur_res) {

        case Result::success:
        {
            if (nt_elapsed[row] > 0)
                nt_utilized[row] = true;
            pos = cur_cell->pos();
            return true;
        }
        case Result::fail:
        {
            if (nt_elapsed[row] > 0)
                nt_utilized[row] = true;
            return false;
        }
        case Result::unknown:
        {
            if (nt_elapsed[row] == 0)
                if (!nt_utilized[row])
                    nt_activated[row] = false;

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
//    std::cout << "Parsing..." << std::endl;
    NonTerminal* nt;

    nt = peg.get_start();
    return nt->accept(*this);
}

void Elastic::print_active() const
{
    auto N = peg.get_rules().size();

    std::cout << std::endl;
    for(int i = 0; i < N; ++i) {
        std::cout << nt_activated[i] << " ";
    }
    std::cout << std::endl;
}

unsigned int hash(unsigned int x) // TODO: further work
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}
