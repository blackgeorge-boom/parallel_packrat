//
// Created by blackgeorge on 18/3/20.
//

#include "cmath"
#include <mutex>
#include <thread>

#include "conc_elastic_packrat.h"
#include "conc_elastic_worker.h"

ConcurrentElasticPackrat::ConcurrentElasticPackrat(std::string input, const PEG& g, int window_size, int threshold, int lim)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);

    width = window_size;
    nt_num = peg.get_rules().size();
    thres= threshold;
    expr_limit = lim;

    shift = ceil(log2(nt_num));

    rank = -1;

    nt_elapsed = new int[nt_num];
    nt_utilized = new int[nt_num];
    nt_activated = new int[nt_num];
    for (auto i = 0; i < nt_num; ++i) {
        nt_elapsed[i] = thres;
        nt_utilized[i] = false;
        nt_activated[i] = true;
    }

    elastic_cells = new ElasticCell[width * nt_num];
}

bool ConcurrentElasticPackrat::visit(NonTerminal& nt)
{
    int row = nt.index();

    if (!nt_activated[row]) {
        Expression* e = peg.get_expr(&nt);
        return e->accept(*this);
    }

    if (nt_elapsed[row] >= 0)
        nt_elapsed[row] = nt_elapsed[row] - 1;

    long int key = (pos << shift) | row;
    unsigned int index = hash(key) % (width * nt_num);

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

            cur_cell->set_key(key);

            auto res = e->accept(*this);
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

bool ConcurrentElasticPackrat::visit(CompositeExpression& ce)
{
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
            if (exprs.size() > expr_limit) {    // Parse without spawning threads
                for (auto expr : exprs) {
                    pos = orig_pos;
                    if (expr->accept(*this))
                        return true;
                }
                pos = orig_pos;
                return false;
            }

            std::vector<bool> results;
            std::vector<int> positions;
            std::vector<std::thread> threads;
            std::vector<ConcurrentElasticWorker*> workers;

            finished_rank.store(-1);

            auto i = 0;
            for (auto& expr : exprs) { // TODO: maybe add restriction when expr.size > 4. also pht table
                workers.emplace_back(new ConcurrentElasticWorker(in, pos, peg, width, thres, i,
                        nt_elapsed, nt_utilized, nt_activated, elastic_cells));
                threads.emplace_back([&, expr, i, this]()
                                     {
                                         results.push_back(expr->accept(*workers[i]));
                                         positions.push_back((*workers[i]).cur_pos());
                                     }
                );
                i++;
            }

            auto j = 0;
            for (auto w : workers) {
                threads[j].join(); // TODO: Reverse with I by compiler?
                delete workers[j];
                if (results[j]) { // TODO: I?
//                    std::cout << "I is: " << i << " and J is: " << j << std::endl;
                    finished_rank.store(j);
                    pos = positions[j];
                    for (auto k = j + 1; k < workers.size(); ++k) {
                        threads[k].join();
//                        workers[k]->stop();
                        delete workers[k];
                    }
                    return true;
                }
                j++;
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

bool ConcurrentElasticPackrat::visit(PEG& peg)
{
//    std::cout << "Parsing..." << std::endl;
    NonTerminal* nt;

    nt = peg.get_start();
    return nt->accept(*this);
}
