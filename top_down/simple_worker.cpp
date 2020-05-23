//
// Created by blackgeorge on 11/7/2019.
//

#include <cassert>

#include <mutex>
#include <thread>

#include "simple_worker.h"


std::atomic<int> finished_rank;

SimpleWorker::SimpleWorker(std::string input, const PEG& g, Cell** c, int p, int lim, int depth)
{
    in = std::move(input);
    peg = PEG(g);
    cells = c;
    pos = p;
    expr_limit = lim;
    cur_tree_depth = depth;
}

bool SimpleWorker::visit(NonTerminal &nt)
{
//    std::cout << std::this_thread::get_id() << std::endl;
//    if (this->stopRequested()) {
//        std::cout << "Stopped\n";
//        return false;
//    }

    int row = nt.index();
    Cell* cur_cell = &cells[row][pos];

    cur_cell->lock();
    Result cur_res = cur_cell->res();
    cur_cell->unlock();

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
//                std::cout << "Thread " << std::this_thread::get_id() <<
//                " of " << nt << " stuck at " << pos << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            assert (cur_cell->res() != Result::pending);
            if (cur_cell->res() == Result::success) {
                pos = cur_cell->pos();
                return true;
            }
            return false;
        }
        case Result::unknown:
        {
            cur_cell->lock();
            cur_cell->set_res(Result::pending);
            cur_cell->unlock();
            Expression* e = peg.get_expr(&nt);
            auto res = e->accept(*this); // TODO: check

            if (res) {
                cur_cell->set_pos(pos); // pos has changed
                cur_cell->set_res(Result::success);
                return true;
            } else {
                cur_cell->set_res(Result::fail);
                return false;
            }
        }
    }
}

bool SimpleWorker::visit(CompositeExpression &ce)
{
//    if (this->stopRequested()) {
//        std::cout << "Stopped\n";
//        return false;
//    }

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
            if (exprs.size() > expr_limit || cur_tree_depth > 4) {    // Parse without spawning threads
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
                workers.emplace_back(new SimpleWorker{in, peg, cells, pos, expr_limit, cur_tree_depth + 1});
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
                threads[j].join();
                if (results[j]) {
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

bool SimpleWorker::visit(Terminal& t)
{
//    if (this->stopRequested()) {
//        std::cout << "Stopped\n";
//        return false;
//    }

    int terminal_char = t.name()[0];

    if (t.name().length() > 1)
        terminal_char = hex2dec(t.name().substr(2, 4));

    if (pos < in.size() && terminal_char == this->cur_tok()) {
        pos++;
        return true;
    }
    return false;
}

