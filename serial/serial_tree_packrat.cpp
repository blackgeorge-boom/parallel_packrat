//
// Created by blackgeorge on 4/19/19.
//

#include "serial_tree_packrat.h"

SerialTreePackrat::SerialTreePackrat(const char* input, PEG g)
    : SerialPackrat(input, g)
{
    auto nt = new NonTerminal("(Start)");
    root = new TreeNode(nt);
    tree_pos = root;
}


SerialTreePackrat::SerialTreePackrat(std::string input, PEG g)
    : SerialPackrat(input, g)
{
    auto nt = new NonTerminal("(Start)");
    root = new TreeNode(nt);
    tree_pos = root;
}

bool SerialTreePackrat::visit(NonTerminal& nt)
{
    int row = nt.index();
    Cell* cur_cell = &cells[row][pos];
    Result cur_res = cur_cell->res();

    switch (cur_res) {

        case Result::success:
        {
            pos = cur_cell->pos();
            tree_pos->push_child(cur_cell->get_node());
            return true;
        }
        case Result::fail:
        {
            return false;
        }
        case Result::unknown:
        {
            Expression *e = peg.get_expr(&nt);
            auto old_tree_pos = tree_pos;
            tree_pos = new TreeNode(&nt);
            auto res = e->accept(*this);

            if (res) {
                cur_cell->set_res(Result::success);
                cur_cell->set_pos(pos); // pos has changed
                cur_cell->set_node(tree_pos);

//                std::cout << "========================================\n";
//                std::cout << nt << " -> " << *e << " \n\n";
//                printTree("", tree_pos, true);
                old_tree_pos->push_child(tree_pos);
                tree_pos = old_tree_pos;
//                printTree("", tree_pos, true);
                return true;
            }
            else {
                cur_cell->set_res(Result::fail);
                tree_pos = old_tree_pos;
                return false;
            }
        }
    }
}

bool SerialTreePackrat::visit(Terminal& t)
{
    if (pos < in.size() && t.name()[0] == this->cur_tok()) {
        pos++;
        tree_pos->push_child(new TreeNode(&t));
        return true;
    }
    return false;
}

bool SerialTreePackrat::visit(CompositeExpression& ce)
{
    char op = ce.op_name();
    std::vector<Expression*> exprs = ce.expr_list();
    auto orig_pos = pos;
    auto old_tree_pos = tree_pos;

    switch (op) {
        case '\b':  // sequence
        {
            tree_pos = new TreeNode();
            for (auto expr : exprs) {
                if (!expr->accept(*this)) {
                    pos = orig_pos;
                    tree_pos = old_tree_pos;
                    return false;
                }
            }
            old_tree_pos->push_children(tree_pos->get_children());
            tree_pos = old_tree_pos;
            return true;
        }
        case '/':   // ordered choice
        {
            for (auto expr : exprs) {
                pos = orig_pos;
                tree_pos = new TreeNode();
                if (expr->accept(*this)) {
                    old_tree_pos->push_children(tree_pos->get_children());
                    tree_pos = old_tree_pos;
                    return true;
                }
            }
            pos = orig_pos;
            tree_pos = old_tree_pos;
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

bool SerialTreePackrat::visit(Empty& e)
{
    tree_pos->push_child(new TreeNode(&e));
    return true;
}

bool SerialTreePackrat::visit(AnyChar& ac)
{
    if (pos < in.size()) {
        std::string s(1, this->cur_tok());
        auto t = new Terminal(s);
        tree_pos->push_child(new TreeNode(t));
        pos++;
        return true;
    }
    return false;
}

bool SerialTreePackrat::visit(PEG& peg)
{
//    std::cout << "Parsing..." << std::endl;
    NonTerminal *nt;
    bool res;

    nt = peg.get_start();
    res = nt->accept(*this);

    /*
    int N = peg.get_rules().size();
    int M = in.size() + 1;

    for (auto j = M - 1; j >= 0; --j) {
        for (auto i = N - 1; i >= 0; --i) {
            pos = j;
            nt = peg.get_non_term(i);
            nt->accept(*this);
        }
    }

    res = cells[0][0].res() == Result::success;
     */
    return res;
}
