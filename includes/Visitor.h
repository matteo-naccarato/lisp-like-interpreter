#ifndef VISITOR_H
#define VISITOR_H

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

#include "Program.h"
#include "Block.h"
#include "Statement.h"
#include "NumExpr.h"
#include "Exceptions.h"


/* Superclass visitor made for visit the syntactic tree */
class Visitor {
public:
    // Constants made for identifying a symbolic value for TRUE and FALSE
    static constexpr int TRUE_VAL = 1;
    static constexpr int FALSE_VAL = 0;

    virtual void visitProgram(Program* prg) = 0;
    virtual void visitBlock(Block* blk) = 0;

    virtual void visitSet(SetStmt* s) = 0;
    virtual void visitInput(InputStmt* s) = 0;
    virtual void visitPrint(PrintStmt* s) = 0;
    virtual void visitIf(IfStmt* s) = 0;
    virtual void visitWhile(WhileStmt* s) = 0; 

    virtual void visitOperator(Operator* opNode) = 0;
    virtual void visitNumber(Number* numNode) = 0;
    virtual void visitVariable(Variable* varNode) = 0;

    virtual void visitRelOp(RelOp* rop) = 0;
    virtual void visitBoolConst(BoolConst* bconst) = 0;
    virtual void visitBoolOp(BoolOp* bop) = 0;
};


/* Class that extends Visitor superclass to print the sintactic tree to visit */
class PrintVisitor : public Visitor {
public:
    PrintVisitor() {}
    PrintVisitor(const PrintVisitor& other) = default;
    ~PrintVisitor() = default;
    PrintVisitor& operator=(const PrintVisitor& other) = default;

    void visitProgram(Program* prg) override {
        prg->get_blk()->accept(this);
    }

    void visitBlock(Block* blk) override {
        std::cout << "(BLOCK" << std::endl;
        std::vector<Statement*> stmts = blk->get_stmts();
        auto i = stmts.begin();
        for (; i != stmts.end(); i++) {
            std::cout << "\t";
            (*i)->accept(this);
        }
        std::cout << ")" << std::endl;
    }

    void visitSet(SetStmt* s) override {
        std::cout << "(SET ";
        s->get_var()->accept(this);
        std::cout << " ";
        s->get_nexpr()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitInput(InputStmt* s) override {
        std::cout << "(INPUT ";
        s->get_var()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitPrint(PrintStmt* s) override {
        std::cout << "(PRINT ";
        s->get_nexpr()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitIf(IfStmt* s) override {
        std::cout << "(IF ";
        s->get_bexpr()->accept(this);
        s->get_stmt_block1()->accept(this);
        s->get_stmt_block2()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitWhile(WhileStmt* s) override {
        std::cout << "(WHILE ";
        s->get_bexpr()->accept(this);
        s->get_stmt_block()->accept(this);
        std::cout << ")";
    }

    void visitOperator(Operator* opNode) override {
        std::cout << "(" << Operator::opCode2String(opNode->getOp()) << " ";
        opNode->getFirst()->accept(this);
        std::cout << " ";
        opNode->getSecond()->accept(this);
        std::cout << ")";
    }

    void visitNumber(Number* numNode) override {
        std::cout << numNode->get_value();
    }

    void visitVariable(Variable* varNode) override {
        std::cout << varNode->get_id();
    }

    void visitRelOp(RelOp* rop) override {
        std::cout << "(" << RelOp::relOpCode2String(rop->get_r_opcode()) << " ";
        rop->get_first_nexpr()->accept(this);
        std::cout << " ";
        rop->get_second_nexpr()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitBoolConst(BoolConst* bconst) override {
        std::cout << BoolConst::boolCode2String(bconst->get_bconst());
    }

    void visitBoolOp(BoolOp* bop) override {
        std::cout << "(" << BoolOp::bopCode2String(bop->get_b_opcode()) << " ";
        bop->get_f_bexpr()->accept(this);
        if (bop->get_b_opcode() != BoolOp::NOT) {
            std::cout << " ";
            bop->get_s_bexpr()->accept(this);
        }
        std::cout << ")" << std::endl;
    }
};


/* Class that extends Visitor superclass to visit and evaluate the sintactic tree */
class EvaluationVisitor : public Visitor {
public: 
    EvaluationVisitor() : accumulator{ } {}
    EvaluationVisitor(const EvaluationVisitor& other) = default;
    ~EvaluationVisitor() = default;
    EvaluationVisitor& operator=(const EvaluationVisitor& other) = default;
   
    void visitProgram(Program* prg) override {
        if (prg->get_is_not_empty()) {
            prg->get_blk()->accept(this);
        }
    }

    void visitBlock(Block* blk) override {
        std::vector<Statement*> stmts = blk->get_stmts();
        unsigned int vecSize = stmts.size();
        if (vecSize == 0) throw SemanticError("(ERROR (semantic): empty block given )");
        // Visit of each statement in the analysed block
        for(unsigned int i = 0; i < vecSize; i++) 
            stmts[i]->accept(this);
    } 


    /* STATEMENTS */
    void visitSet(SetStmt* s) override {

        Variable* v = s->get_var(); // VARIABLE
        s->get_nexpr()->accept(this); // NUM_EXPR visit

        int64_t val = accumulator.back(); accumulator.pop_back();
        v->set_value(val);

        // VARIABLE_ID look up 
        std::map<std::string, int64_t>::iterator it = vars.find(v->get_id());
        /* If the variable has been already declared, the value "val" is assigned to it.
        Otherwise it's created. */
        if (it != vars.end()) it->second = val;
        else vars.insert(std::pair<std::string, int64_t>(v->get_id(), v->get_value()));
    }

    void visitInput(InputStmt* s) override {
        std::string tmp_input;
        std::cout << "INPUT \"" << s->get_var()->get_id() << "\": ";
        std::cin >> tmp_input;

        bool valid_input = 1; // flag to verify the correctness of the value given in input
        if (((tmp_input[0] == '0') && tmp_input.size() > 1) 
                || (tmp_input[0] == '-' && tmp_input.size() > 2 && tmp_input[1] == '0' )) {
            // value not valid because is made of a 0 followed by other significant digits
            // or a '-', followed by a 0, followed by significant digits
            valid_input = 0; 
        } else {
            // analysis that each character given in input is a number
            for (char c : tmp_input) {
                if ((c < 48 || c > 57) && c != '-') {
                    valid_input = 0;
                    break;
                }
            }
        }

        if (valid_input) {
            // std::string to int64_t conversion
            int64_t tmp = std::stoll(tmp_input);

            Variable* v = s->get_var();
            /* VARIABLE_ID look up in vars map.
            If it's already there, the value given in input is assigned to it.
            Otherwise the variable is created. */
            std::map<std::string, int64_t>::iterator it = vars.find(v->get_id());
            if (it != vars.end()) it->second = tmp;
            else vars.insert(std::pair<std::string, int64_t>(v->get_id(), tmp));

        } else {
            std::stringstream tmp{};
            tmp << "(ERROR (semantic): input value can only be a NUMBER and significant digits can't be after a series of 0, \"" << tmp_input << "\" given )";
            throw SemanticError(tmp.str());
        }
    }

    void visitPrint(PrintStmt* s) override {
        // dynamic_cast<Variable*> used to verify if the NumExpr* in the statement is a Variable* object: if so, dynamic_cast returns a pointer other than nullptr 
        if (dynamic_cast<Variable*> (s->get_nexpr()) != nullptr) {
            // Look up if a variable has already been declared before (dynamic_cast is necessary to call "get_id()" method of Variable class)
            if (vars.find(dynamic_cast<Variable*> (s->get_nexpr())->get_id()) != vars.end()) 
                s->get_nexpr()->accept(this);
            else {
                std::stringstream tmp{};
                tmp << "(ERROR (semantic): PRINT of VARIABLE_ID \"" << dynamic_cast<Variable*> (s->get_nexpr())->get_id() << "\" not declared before )";
                throw SemanticError(tmp.str());   
            } 
        } else s->get_nexpr()->accept(this);

        // Print of the value using the accumulator
        int64_t val = accumulator.back(); accumulator.pop_back();
        std::cout << val << std::endl;
    }
    
    void visitIf(IfStmt* s) override {
        // bool_expr evaluation
        s->get_bexpr()->accept(this);
        int64_t bexpr_val = accumulator.back(); accumulator.pop_back();
        if (bexpr_val) {
            // "TRUE" visit
            s->get_stmt_block1()->accept(this);
        } else {
            // "FALSE" visit
            s->get_stmt_block2()->accept(this);
        }
    }

    void visitWhile(WhileStmt* s) override {
        // bool_expr evaluation
        s->get_bexpr()->accept(this);
        int64_t bexpr_val = accumulator.back(); accumulator.pop_back();
        // Iteration until bool_expr is "TRUE"
        while (bexpr_val) {
            // Block visit
            s->get_stmt_block()->accept(this);
            // New bool_expr evaluation
            s->get_bexpr()->accept(this);
            bexpr_val = accumulator.back(); accumulator.pop_back();
        }
    }


    /* NUM_EXPR */
    void visitOperator(Operator* opNode) override {
        Operator::OpCode op_code = opNode->getOp();
        NumExpr* first = opNode->getFirst();

        // dynamic_cast<Variable*> used to verify if the NumExpr* in statement is a Variable* object: if so, dynamic_cast returns a pointer other than nullptr
        if (dynamic_cast<Variable*> (first) != nullptr) {
            // Look up if the variable has already been declared before (dynamic_cast is needed to call "get_id()" method of the Variable class)
            if (vars.find(dynamic_cast<Variable*> (first)->get_id()) != vars.end()) 
                first->accept(this);
            else {
                std::stringstream tmp{};
                tmp << "(ERROR (semantic): ARITHMETIC_OPERATOR \"" << Operator::opCode2String(op_code) << "\" is using VARIABLE_ID \"" << dynamic_cast<Variable*> (first)->get_id() << "\" not declared before )";
                throw SemanticError(tmp.str());   
            } 
        } else first->accept(this);
        

        NumExpr* second = opNode->getSecond();
        // dynamic_cast<Variable*> used to verify if the NumExpr* in statement is a Variable* object: if so, dynamic_cast returns a pointer other than nullptr
        if (dynamic_cast<Variable*> (second) != nullptr) {
            // Look up if the variable has already been declared before (dynamic_cast is needed to call "get_id()" method of the Variable class)
            if (vars.find(dynamic_cast<Variable*> (second)->get_id()) != vars.end()) 
                second->accept(this);
            else {
                std::stringstream tmp{};
                tmp << "(ERROR (semantic): ARITHMETIC_OPERATOR \"" << Operator::opCode2String(op_code) << "\" is using VARIABLE_ID \"" << dynamic_cast<Variable*> (second)->get_id() << "\" not declared before )";
                throw SemanticError(tmp.str());   
            } 
        } else second->accept(this);
        
        // Read the two values calculated from the accumulator
        int64_t sval = accumulator.back();
        accumulator.pop_back();
        int64_t fval = accumulator.back();
        accumulator.pop_back();
        // Compute the result and writing it in the back of the accumulator
        switch (op_code) {
            case Operator::ADD:
                accumulator.push_back(fval + sval); return;
            case Operator::SUB:
                accumulator.push_back(fval - sval); return;
            case Operator::MUL:
                accumulator.push_back(fval * sval); return;
            case Operator::DIV:
                if (sval != 0) accumulator.push_back(fval / sval); 
                else throw SemanticError("(ERROR (semantic): DIV by 0 )"); 
                return;
            default: return; 
        }
    }

    void visitNumber(Number* numNode) override {
        accumulator.push_back(numNode->get_value());
    }

    void visitVariable(Variable* varNode) override {
        // Look up of the variable and writing the value on the accumulator
        std::map<std::string, int64_t>::const_iterator it = vars.find(varNode->get_id());
        if (it != vars.end()) {
            accumulator.push_back(it->second);
        } else {
            std::stringstream tmp{};
            tmp << "(ERROR (semantic): variable \"" << varNode->get_id() << "\" NOT declared before )" << std::endl;
            throw SemanticError(tmp.str());
        }
    }


    /* ============ BOOL_EXPR ============*/
    void visitRelOp(RelOp* rop) override { 
        RelOp::RelOpCode r_opcode = rop->get_r_opcode();
        NumExpr* f_nexpr = rop->get_first_nexpr();

        // dynamic_cast<Variable*> used to verify if the NumExpr* in the statement is a Variable* object: if so, dynamic_cast returns a value other than nullptr
        if (dynamic_cast<Variable*> (f_nexpr) != nullptr) {
            // Look up if the variable has already been declared before (dynamic_cast is needed to call "get_id()" method of the Variable class)
            if (vars.find(dynamic_cast<Variable*> (f_nexpr)->get_id()) != vars.end()) 
                f_nexpr->accept(this); 
            else {
                std::stringstream tmp{};
                tmp << "(ERROR (semantic): REL_OP \"" << RelOp::relOpCode2String(r_opcode) << "\" is using VARIABLE_ID \"" << dynamic_cast<Variable*> (f_nexpr)->get_id() << "\" not declared before )";
                throw SemanticError(tmp.str());   
            } 
        } else f_nexpr->accept(this);

        NumExpr* s_nexpr = rop->get_second_nexpr();
        // dynamic_cast<Variable*> used to verify if the NumExpr* in the statement is a Variable* object: if so, dynamic_cast returns a value other than nullptr
        if (dynamic_cast<Variable*> (s_nexpr) != nullptr) {
            // Look up if the variable has already been declared before (dynamic_cast is needed to call "get_id()" method of the Variable class)
            if (vars.find(dynamic_cast<Variable*> (s_nexpr)->get_id()) != vars.end()) 
                s_nexpr->accept(this);
            else {
                std::stringstream tmp{};
                tmp << "(ERROR (semantic): REL_OP \""  << RelOp::relOpCode2String(r_opcode) << "\" is using VARIABLE_ID \"" << dynamic_cast<Variable*> (s_nexpr)->get_id() << "\" not declared before )";
                throw SemanticError(tmp.str());   
            } 
        } else s_nexpr->accept(this);
        
        // Read from the accumulator of the two values calculated 
        int64_t sval = accumulator.back(); accumulator.pop_back();
        int64_t fval = accumulator.back(); accumulator.pop_back();
        // Writing the values rappresented by the constants TRUE_VAL (1) and FALSE_VAL (0) on the accumulator 
        switch (r_opcode) {
            case RelOp::LT: accumulator.push_back((fval < sval)? TRUE_VAL:FALSE_VAL); return;
            case RelOp::GT: accumulator.push_back((fval > sval)? TRUE_VAL:FALSE_VAL); return;
            case RelOp::EQ: accumulator.push_back((fval == sval)? TRUE_VAL:FALSE_VAL); return;
            default: break;
        }
    }

    void visitBoolConst(BoolConst* bconst) override {
        // Writing the values rappresented by the constants TRUE_VAL (1) and FALSE_VAL (0) on the accumulator 
        if (BoolConst::boolCode2String(bconst->get_bconst()) == Token::id2word[Token::TRUE]) accumulator.push_back(TRUE_VAL);
        if (BoolConst::boolCode2String(bconst->get_bconst()) == Token::id2word[Token::FALSE]) accumulator.push_back(FALSE_VAL);
    }

    void visitBoolOp(BoolOp* bop) override {
        // first BoolExpr evaluation
        BoolExpr* f_bexpr = bop->get_f_bexpr();
        f_bexpr->accept(this); 
        int64_t fval = accumulator.back(); accumulator.pop_back();
        // Flag to perform short-circuit of boolean operators and evaluate NOT clause
        bool done = false;
        // 1st switch to short-circuit the boolean operator and evaluate NOT clause
        switch (bop->get_b_opcode()) {
            case BoolOp::AND:
                if (!fval) {
                    accumulator.push_back(false);
                    done = true; 
                }
                break;
            case BoolOp::OR: 
                if (fval) {
                    accumulator.push_back(true); 
                    done = true;
                }
                break;
            case BoolOp::NOT: 
                accumulator.push_back(!fval);
                done = true;
                break;
        }

        // Evaluation, only if necessary, of the 2nd BoolExpr
        if (!done) { 
            BoolExpr* s_bexpr = bop->get_s_bexpr();
            s_bexpr->accept(this);
            int64_t sval = accumulator.back(); accumulator.pop_back();
            
            // Writing the result of the boolean operation on the accumulator
            switch (bop->get_b_opcode()) {
                case BoolOp::AND: accumulator.push_back(fval && sval); break;
                case BoolOp::OR: accumulator.push_back(fval || sval); break;
                case BoolOp::NOT: accumulator.push_back(!fval);
            }
            done = true;
        }
    }

private:
    std::vector<int64_t> accumulator; // stack method to store int values (int64_t) 
    std::map<std::string, int64_t> vars; // use of std::map to store and manage the variables as combination of a std::string (VARIABLE_ID) and an int64_t (VARIABLE_VALUE)
};

#endif /* VISITOR_H */