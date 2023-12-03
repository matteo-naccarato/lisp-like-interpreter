#ifndef BOOL_EXPR_MANAGER_H
#define BOOL_EXPR_MANAGER_H

#include <vector>

#include "BoolExpr.h"


/* BoolExpr manager class used to create and deallocate the BoolExpr objects, stored in an only attribute std::vector made of pointers to BoolExpr objects */
class BoolExprManager {
public:
    BoolExprManager() = default;
    ~BoolExprManager() { clear_memory(); }

    /* Deletion of the copy constructor and the assignment operator to avoid pointers ownership errors */
    BoolExprManager(const BoolExprManager& other) = delete;
    BoolExprManager& operator=(const BoolExprManager& other) = delete;


    BoolExpr* makeRelOp(RelOp::RelOpCode rop, NumExpr* f_expr, NumExpr* s_expr) {
        BoolExpr* bexpr = new RelOp(rop, f_expr, s_expr);
        bool_expr_allocated.push_back(bexpr);
        return bexpr;
    }

    BoolExpr* makeBoolConst(BoolConst::BoolCode bcode) {
        BoolExpr* bexpr = new BoolConst(bcode);
        bool_expr_allocated.push_back(bexpr);
        return bexpr;
    }

    BoolExpr* makeBoolOp(BoolOp::BopCode bcode, BoolExpr* f_bexpr, BoolExpr* s_bexpr) {
        BoolExpr* bexpr = new BoolOp(bcode, f_bexpr, s_bexpr);
        bool_expr_allocated.push_back(bexpr);
        return bexpr;
    }

    void clear_memory() {
        auto i = bool_expr_allocated.begin();
        for (; i != bool_expr_allocated.end(); i++) {
            delete(*i);
        }
        bool_expr_allocated.resize(0);
    }

private:
    std::vector<BoolExpr*> bool_expr_allocated;
};


#endif /* BOOL_EXPR_MANAGER_H */