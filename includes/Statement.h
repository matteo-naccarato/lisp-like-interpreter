#ifndef STATEMENT_H
#define STATEMENT_H

#include <iostream>

#include "NumExpr.h"
#include "BoolExpr.h"


class Block;


/* Statement superclass to identify the possible instructions */
class Statement {
public:
    virtual ~Statement() {};
    virtual void accept(Visitor* v) = 0;
};


/* Class used to identify the instruction (SET variable_id num_expr) */
class SetStmt : public Statement {
public:
    SetStmt(NumExpr* ne, Variable* va) : nexpr{ ne }, var{ va } {};
    SetStmt(const SetStmt& other) = default;
    ~SetStmt() = default;

    NumExpr* get_nexpr() const { return nexpr; }
    Variable* get_var() const { return var; }
    void set_var(Variable* v) { var = v; }

    void accept(Visitor* v) override;

private:
    NumExpr* nexpr;
    Variable* var;
};


/* Class used to identify the instruction (INPUT variable_id) */
class InputStmt : public Statement {
public:
    InputStmt(Variable* v) : var{ v } {};
    InputStmt(const InputStmt& other) = default;
    ~InputStmt() = default;

    Variable* get_var() const { return var; }

    void accept(Visitor* v) override;

private:
    Variable* var; 
};


/* Class used to identify the instruction (PRINT num_expr) */
class PrintStmt : public Statement {
public:    
    PrintStmt(NumExpr* ne) : nexpr{ ne } {};
    PrintStmt(const PrintStmt& other) = default;
    ~PrintStmt() = default;

    NumExpr* get_nexpr() const { return nexpr; }
    
    void accept(Visitor* v) override;
    
private:
    NumExpr* nexpr;
};


/* CLass used to identify the instruction (IF bool_expr stmt_block1 stmt_block2) */
class IfStmt : public Statement {
public:
    IfStmt(BoolExpr* be, Block* b1, Block* b2) : bexpr{ be }, stmt_block1{ b1 }, stmt_block2{ b2 } {};
    IfStmt(const IfStmt& other) = default;
    ~IfStmt() = default;

    BoolExpr* get_bexpr() const { return bexpr; }
    Block* get_stmt_block1() const { return stmt_block1; }
    Block* get_stmt_block2() const { return stmt_block2; }

    void accept(Visitor* v) override;

private:
    BoolExpr* bexpr;
    Block* stmt_block1;
    Block* stmt_block2;
};


/* Class used to identify the instruction (WHILE bool_expr stmt_block) */
class WhileStmt : public Statement {
public:
    WhileStmt(BoolExpr* be, Block* sb) : bexpr{ be }, stmt_block{ sb } {};
    WhileStmt(const WhileStmt& other) = default;
    ~WhileStmt() = default;

    BoolExpr* get_bexpr() const { return bexpr; }
    Block* get_stmt_block() const { return stmt_block; }

    void accept(Visitor* v) override;

private:
    BoolExpr* bexpr;
    Block* stmt_block;
};


#endif /* STATEMENT_H */