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
            if (exprs.size() > expr_limit) {    // Parse without spawning threads
                for (auto expr : exprs) {
                    pos = orig_pos;
                    if (expr->accept(*this))
                        return true;
                }
                pos = orig_pos;
                return false;
            }

            int results[exprs.size()];
            int positions[exprs.size()];
            std::vector<std::thread> threads;
            std::vector<std::shared_ptr<SimpleWorker>> workers;

            auto i = 0;
            for (auto& expr : exprs) {
                workers.emplace_back(new SimpleWorker{in, peg, cells, pos, 0, 0});
                threads.emplace_back([&, expr, i]()
                                     {
                                         bool res = expr->accept(*workers[i]);
                                         results[i] = res;
                                         positions[i] = workers[i]->cur_pos();
                                     }
                );
                i++;
            }

            for (auto j = 0; j < i; ++j) {
                threads[j].join(); // TODO: Reverse with 'I' by compiler?
                if (results[j]) { // TODO: 'I'
//                    finished_rank.store(j);
                    pos = positions[j];
                    for (auto k = j + 1; k < i; ++k) {
                        workers[k]->stop();
                        threads[k].join();
                    }
                    return true;
                }
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
