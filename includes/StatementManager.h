#ifndef STATEMENT_MANAGER_H
#define STATEMENT_MANAGER_H

#include <vector>

#include "Statement.h"


/* Class Statement manager used to manager the creation and deletion of Statement objects */
class StatementManager {
public:
    StatementManager() = default;
    ~StatementManager() { clear_memory(); }

    /* Deletion of copy constructor and assignment operator to avoid pointers ownership errors */
    StatementManager(const StatementManager& other) = delete;
    StatementManager& operator=(const StatementManager& other) = delete;

    Statement* makeSetStmt(NumExpr* nexpr, Variable* var) {
        Statement* s = new SetStmt(nexpr, var);
        stmts_allocated.push_back(s);
        return s;
    }

    Statement* makeInputStmt(Variable* v) {
        Statement* s = new InputStmt(v);
        stmts_allocated.push_back(s);
        return s;
    }

    Statement* makePrintStmt(NumExpr* nexpr) {
        Statement* s = new PrintStmt(nexpr);
        stmts_allocated.push_back(s);
        return s;
    }

    Statement* makeIfStmt(BoolExpr* bexpr, Block* stmt_blk1, Block* stmt_blk2) {
        Statement* s = new IfStmt(bexpr, stmt_blk1, stmt_blk2);
        stmts_allocated.push_back(s);
        return s;
    }

    Statement* makeWhileStmt(BoolExpr* bexpr, Block* stmt_blk) {
        Statement* s = new WhileStmt(bexpr, stmt_blk);
        stmts_allocated.push_back(s);
        return s;
    }

    void clear_memory() {
        auto i = stmts_allocated.begin();
        for (; i != stmts_allocated.end(); i++) {
            delete(*i);  
        } 
        stmts_allocated.resize(0);
    }

private:
    std::vector<Statement*> stmts_allocated;
};

#endif /* STATEMENT_MANAGER */