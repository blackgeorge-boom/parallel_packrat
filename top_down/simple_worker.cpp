//
// Created by blackgeorge on 11/7/2019.
//

#include <cassert>

#include <mutex>
#include <thread>

#include "simple_worker.h"


SimpleWorker::SimpleWorker(std::string input, const PEG& g, Cell** c, int p, int lim, int cur_depth, int max_depth, int r, std::atomic<int>* pfr)
{
    in = std::move(input);
    peg = PEG(g);
    cells = c;
    pos = p;
    expr_limit = lim;
    cur_tree_depth = cur_depth;
    max_tree_depth = max_depth;
    rank = r;
    parent_finished_rank = pfr;
}

bool SimpleWorker::visit(NonTerminal &nt)
{
    auto fr = parent_finished_rank->load();
    if (fr >= 0 && fr < rank) {
//        std::cout << "Stopped at: " << nt << "\n";
        return false;
    }

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
                std::this_thread::sleep_for(std::chrono::milliseconds(0));
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
            }
            else {
                cur_cell->set_res(Result::fail);
                return false;
            }
        }
    }
}

bool SimpleWorker::visit(CompositeExpression &ce)
{
    auto fr = parent_finished_rank->load();
    if (fr >= 0 && fr < rank) {
//        std::cout << "Stopped at: " << nt << "\n";
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
            if (exprs.size() > expr_limit || cur_tree_depth >= max_tree_depth) {    // Parse without spawning threads
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

            auto i = 0;
            for (auto& expr : exprs) {
                threads.emplace_back([&, expr, i]()
                                     {
                                         SimpleWorker sw{in, peg, cells, pos, expr_limit, cur_tree_depth + 1, max_tree_depth, i, &finished_rank};
                                         int res = expr->accept(sw);
                                         results[i] = res;
                                         positions[i] = sw.cur_pos();
                                     }
                );
                i++;
            }

            for (auto j = 0; j < i; ++j) {
                threads[j].join();
                if (results[j]) {
                    finished_rank.store(j);
                    pos = positions[j];
                    for (auto k = j + 1; k < i; ++k) {
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
    int terminal_char = t.name()[0];

    if (t.name().length() > 1)
        terminal_char = hex2dec(t.name().substr(2, 4));

    if (pos < in.size() && terminal_char == this->cur_tok()) {
        pos++;
        return true;
    }
    return false;
}

