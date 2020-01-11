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
            auto i = 0;

            std::vector<bool> results;
            std::vector<int> positions;
            std::vector<SimpleWorker*> workers;
            std::vector<std::thread> threads;
            std::vector<Expression*> other_exprs;

            for (auto& expr : exprs) {
                if (peg.get_history(expr)) { // TODO: better with negation
                    workers.emplace_back(new SimpleWorker(in, peg, cells, pos));
                    threads.emplace_back([&, expr, i, this]()
                                        {
                                            results.push_back(expr->accept(*workers[i]));
                                            positions.push_back((*workers[i]).cur_pos());
                                            other_exprs.push_back(expr);
                                        }
                    );
                    i++;
                }
                else {
                    if (expr->accept(*this)) {
                        for (auto w : workers)
                            w->stop();
                        for (auto& t : threads)
                            t.join();
                        pos = this->cur_pos();
                        peg.push_history(expr, true);
                        return true;
                    }
                }
                peg.push_history(expr, false);
            }

            auto j = 0;
            for (auto w : workers) {
                threads[j].join();
                delete workers[j];
                if (results[j]) {
                    pos = positions[j];
                    for (auto k = j + 1; k < workers.size(); ++k) {
                        workers[k]->stop();
                        threads[k].join();
                        delete workers[k];
                    }
                    peg.push_history(other_exprs[j], true);
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

bool TableParallel::visit(NonTerminal &nt)
{
    int row = nt.index();
    Cell* cur_cell = &cells[row][pos];
    Result cur_res = cur_cell->res();

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
        case Result::pending:
        {
            while (cur_cell->res() == Result::pending) {
                std::cout << "Stuck: " << std::this_thread::get_id() <<
                          " " << row << ", " << pos << " " << nt << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            if (cur_cell->res() == Result::pending)
                std::cout << "wuuut" << std::endl;
            return cur_cell->res() == Result::success;
        }
        case Result::unknown:
        {
            cur_cell->lock();
            cur_cell->set_res(Result::pending);
            cur_cell->unlock();
            Expression* e = peg.get_expr(&nt);
            auto res = e->accept(*this); // TODO: check

            if (res) {
                cur_cell->set_res(Result::success);
                cur_cell->set_pos(pos); // pos has changed
                return true;
            } else {
                cur_cell->set_res(Result::fail);
                return false;
            }
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
