//
// Created by blackgeorge on 18/3/20.
//

#include <cmath>
#include <thread>

#include "conc_elastic_worker.h"

ConcurrentElasticWorker::ConcurrentElasticWorker(std::string input, int p, const PEG& g, int window_size, int threshold, int i,
                                                 int* elapsed, int* utilized, int* activated, ElasticCell* et)
{
    in = std::move(input);
    pos = p;
    peg = PEG(g);

    width = window_size;
    nt_num = peg.get_rules().size();
    thres= threshold;

    shift = ceil(log2(nt_num));

    rank = i;

    nt_elapsed = elapsed;
    nt_utilized = utilized;
    nt_activated = activated;

    elastic_cells = et;
}

bool ConcurrentElasticWorker::visit(NonTerminal& nt)
{
//    std::cout << std::this_thread::get_id() << std::endl;
    auto fr = finished_rank.load();
    if (fr > 0) { // TODO: check print
        std::cout << "rank: " << rank << std::endl;
        return false;
    }
    int row = nt.index();

    if (!nt_activated[row]) {
        Expression* e = peg.get_expr(&nt);
        return e->accept(*this);
    }

    if (nt_elapsed[row] >= 0)   // TODO: maybe lock it first
        nt_elapsed[row] = nt_elapsed[row] - 1;

    long int key = (pos << shift) | row;
    unsigned int index = hash(key) % (width * nt_num);

    ElasticCell* cur_cell = &elastic_cells[index];
    Result cur_res = cur_cell->res();

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
            if (nt_elapsed[row] <= 0)
                if (!nt_utilized[row])
                    nt_activated[row] = false;

            Expression* e = peg.get_expr(&nt);

            auto res = e->accept(*this);

            cur_cell->lock();

            cur_cell->set_key(key);
            if (res) {

                cur_cell->set_res(Result::success);
                cur_cell->set_pos(pos); // pos has changed
                cur_cell->unlock();

                return true;
            }
            else {

                cur_cell->set_res(Result::fail);
                cur_cell->unlock();

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

bool ConcurrentElasticWorker::visit(CompositeExpression &ce)
{

    auto fr = finished_rank.load();
    if (fr > 0) { // TODO: check print
        std::cout << "-rank: " << rank << std::endl;
        return false;
    }

    char op = ce.op_name();
    std::vector<Expression*> exprs = ce.expr_list();
    int orig_pos = pos;

    switch (op) {

        case '\b':  // sequence
        {
            for (auto expr : exprs)
                if (!expr->accept(*this)) {
                    pos = orig_pos;
                    return false;
                }
            return true;
        }
        case '/':   // ordered choice
        {
            for (auto expr : exprs) {
                pos = orig_pos;
                if (expr->accept(*this))
                    return true;
            }
            pos = orig_pos;
            return false;
        }
        case '&':   // followed by
        {
            auto res = exprs[0]->accept(*this);
            pos = orig_pos;
            return res;
        }
        case '!':   // not followed by
        {
            auto res = exprs[0]->accept(*this);
            pos = orig_pos;
            return !res;
        }
        case '?':   // optional
        {
            exprs[0]->accept(*this);
            return true;
        }
        case '*':   // repetition
        {
            while (exprs[0]->accept(*this)) ;
            return true;
        }
        case '+':   // positive repetition
        {
            if (!exprs[0]->accept(*this))
                return false;
            while (exprs[0]->accept(*this)) ;
            return true;
        }
        default:
        {
            std::cout << "Visiting not handled!";
            return false;
        }
    }
}
