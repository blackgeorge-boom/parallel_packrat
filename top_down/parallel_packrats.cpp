//
// Created by blackgeorge on 8/16/19.
//

#include <mutex>
#include <thread>

#include <tbb/task_group.h>
#include <tbb/task_scheduler_init.h>

#include "parallel_packrats.h"
#include "simple_worker.h"

bool TableParallel::visit(CompositeExpression& ce)
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
            if (exprs.size() > 8) {    // Parse without spawning threads
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
            std::vector<SimpleWorker*> workers;

            finished_rank.store(-1);

            auto i = 0;
            for (auto& expr : exprs) { // TODO: maybe add restriction when expr.size > 4. also pht table
                workers.emplace_back(new SimpleWorker(in, peg, cells, pos, i));
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

bool TableParallel::visit(PEG& p)
{
    std::cout << "Parsing..." << std::endl;
    NonTerminal *nt;
    bool res;

    nt = peg.get_start();
    nt->accept(*this);
    res = cells[0][0].res() == Result::success;
    return res;
}
