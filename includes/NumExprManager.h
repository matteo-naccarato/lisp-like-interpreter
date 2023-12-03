#ifndef NUM_EXPR_MANAGER_H
#define NUM_EXPR_MANAGER_H

#include <vector>

#include "NumExpr.h"


/* NumExpr manager class to manage their creation and the deallocation,
which are stored in the attribute std::vector containing pointers to NumExpr objects */
class NumExprManager {
public:
    NumExprManager() = default;
    ~NumExprManager() { clear_memory(); }

    /* Deletion of copy constructor and assignment operator to avoid pointers ownership errors */
    NumExprManager(const NumExprManager& other) = delete;
    NumExprManager& operator=(const NumExprManager& other) = delete;
    
    NumExpr* makeOperator(Operator::OpCode op, NumExpr* l, NumExpr* r) {
        NumExpr* ne = new Operator(op, l, r);
        numExpr_allocated.push_back(ne);
        return ne;
    }

    NumExpr* makeNumber(int64_t v) {
        NumExpr* ne = new Number(v);
        numExpr_allocated.push_back(ne);
        return ne;
    }

    NumExpr* makeVariable(std::string id) {
        NumExpr* ne = new Variable(id);
        numExpr_allocated.push_back(ne);
        return ne;
    }


    void clear_memory() {
        auto i = numExpr_allocated.begin();
        for (; i != numExpr_allocated.end(); i++) {
            delete(*i);
        }
        numExpr_allocated.resize(0);
    }

private:
    std::vector<NumExpr*> numExpr_allocated;

};


#endif /* NUM_EXPR_MANAGER_H */