//
// Created by blackgeorge on 8/16/19.
//

#include <mutex>
#include <thread>

#include <tbb/task_group.h>
#include <tbb/task_scheduler_init.h>

#include "parallel_packrats.h"
#include "simple_worker.h"

std::mutex cout_mutex;

bool TableParallel::visit(CompositeExpression& ce)
{
    char op = ce.op_name();
    std::vector<Expression*> exprs = ce.expr_list();
    int orig_pos = pos;

    tbb::task_scheduler_init init(1);

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
            tbb::task_group g;

            auto i = 0;
            std::vector<bool> results;
            std::vector<int> positions;
            std::vector<std::pair<Expression*, int>> rest;

            // TODO: check
            auto local_pos = pos;

            for (auto expr : exprs) {
                std::cout << std::this_thread::get_id() << ", " <<  *expr << std::endl;
                g.run([&]()
                      {
                          SimpleWorker sw{in, peg, cells, local_pos};
                          cout_mutex.lock();
                          std::cout << std::this_thread::get_id() << ", " <<  *expr << std::endl;
                          cout_mutex.unlock();
                          results.push_back(expr->accept(sw));
                          positions.push_back(sw.cur_pos());
                      }
                );
            }

            g.wait();

            for (auto j = 0; j < exprs.size(); ++j)
                if (results[j]) {
                    pos = positions[j];
                    std::cout << "Success! " << pos << std::endl;
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

bool TableParallel::visit(PEG& p)
{
    std::cout << "Parsing..." << std::endl;
    NonTerminal *nt;
    bool res;

    nt = peg.get_start();
    nt->accept(*this);
    res = cells[0][0].res() == Result::success;
    print_cells();
    return res;
}
