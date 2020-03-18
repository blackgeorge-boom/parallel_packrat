//
// Created by blackgeorge on 18/3/20.
//

#include "conc_elastic_base.h"

void ConcurrentElasticBase::print_active() const
{
    auto N = peg.get_rules().size();

    std::cout << std::endl;
    for(int i = 0; i < N; ++i) {
        std::cout << nt_activated[i] << " ";
    }
    std::cout << std::endl;
}

void ConcurrentElasticBase::addData(const long int &key)
{
    ElasticTable::accessor a;
    auto ec = new ElasticCell();
    table->insert(a, key);
    a->second = *ec;
    a.release();
}

bool ConcurrentElasticBase::visit(NonTerminal& nt)
{
    int row = nt.index();

    if (!nt_activated[row]) {
        Expression* e = peg.get_expr(&nt);
        return e->accept(*this);
    }

    if (nt_elapsed[row] >= 0)
        nt_elapsed[row] = nt_elapsed[row] - 1;

    long int key = (pos << shift) | row;

    auto cur_cell = new ElasticCell();
    auto cur_res = Result::unknown;

    ElasticTable::accessor a;

    if (table->find(a, key)) {
        cur_cell = &a->second;
        cur_res = cur_cell->res();
    }
    else {
        this->addData(key);
    }

    if (cur_cell->get_key() != key) {
        cur_res = Result::unknown;
    }

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
