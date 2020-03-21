//
// Created by blackgeorge on 18/3/20.
//

#include "cmath"
#include <mutex>
#include <thread>

#include <tbb/task_scheduler_init.h>
#include "tbb/task_group.h"

#include "conc_elastic_packrat.h"
#include "conc_elastic_worker.h"

ConcurrentElasticPackrat::ConcurrentElasticPackrat(std::string input, const PEG& g, int window_size, int threshold)
{
    in = std::move(input);
    pos = 0;
    peg = PEG(g);

    width = window_size;
    nt_num = peg.get_rules().size();
    thres= threshold;

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

    table = new ElasticTable();
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
            std::vector<bool> results;
            std::vector<int> positions;
            std::vector<ConcurrentElasticWorker*> workers;

//            tbb::task_scheduler_init tbb_init{4};
            tbb::task_group g;

            finished_rank.store(-1);

            auto i = 0;
            for (auto& expr : exprs) { // TODO: maybe add restriction when expr.size > 4. also pht table
                workers.emplace_back(new ConcurrentElasticWorker(in, pos, peg, width, thres, i,
                        nt_elapsed, nt_utilized, nt_activated, table));
                g.run([&, expr, i, this]()
                                     {
                                         results.push_back(expr->accept(*workers[i]));
                                         positions.push_back((*workers[i]).cur_pos());
                                     }
                );
                i++;
            }

            g.wait();
            auto j = 0;
            for (auto w : workers) {
                delete workers[j];
                if (results[j]) {
                    pos = positions[j];
                    for (auto k = j + 1; k < workers.size(); ++k) {
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
    std::cout << "Parsing..." << std::endl;
    NonTerminal* nt;

    nt = peg.get_start();
    return nt->accept(*this);
}
