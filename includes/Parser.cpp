#include <sstream>

#include <iostream>


#include "Token.h"
#include "Parser.h"


/* Recursive method used to parse numerical expressions (NumExpr).
It returns a pointer to a NumExpr object */
NumExpr* ParseProgram::parseNumExpr(std::vector<Token>::const_iterator& itr) {
    if (itr->tag == Token::LP) {

        safe_next(itr);
        NumExpr* nexpr = parseNumExpr(itr);
        safe_next(itr);
        if (itr->tag != Token::RP) {
            throw SyntaxError("(ERROR (syntax): Mismatched parenthesis )");
        }
        return nexpr;

    } else if (itr->tag == Token::NUM) {
        std::stringstream tmp{};
        tmp << itr->word;
        int64_t val;
        tmp >> val;
        return nem.makeNumber(val);

    } else if (itr->tag == Token::VARIABLE_ID) {
        return nem.makeVariable(itr->word); 

    } else if (isOperator(itr->tag)) {
        Operator::OpCode op = Operator::string2OpCode(itr->word);
        safe_next(itr);
        NumExpr* first = parseNumExpr(itr);
        safe_next(itr);
        NumExpr* second = parseNumExpr(itr);
        return nem.makeOperator(op, first, second);
    } else {
        std::stringstream tmp{};
        tmp << "(ERROR (syntax): unexpected token, \"" << itr->word << "\" given )";
        throw SyntaxError(tmp.str());
    }
}


/* Recursive method used to parse boolean expressions (BoolExpr).
It returns a pointer to a BoolExpr object */
BoolExpr* ParseProgram::parseBoolExpr(std::vector<Token>::const_iterator& itr) {
    if (itr->tag == Token::LP) {
        safe_next(itr);
        BoolExpr* bexpr = parseBoolExpr(itr);
        safe_next(itr);

        if (itr->tag != Token::RP) {
            throw SyntaxError("(ERROR (syntax): Mismatched parenthesis )");
        }
        return bexpr;

    } else if (isRelOp(itr->tag)) {
        // GT, LT, EQ
        RelOp::RelOpCode bop = RelOp::string2RelOpCode(itr->word);
        safe_next(itr);
        NumExpr* f_nexpr = parseNumExpr(itr);
        
        safe_next(itr);
        NumExpr* s_nexpr = parseNumExpr(itr);
        
        return bem.makeRelOp(bop, f_nexpr, s_nexpr);

    } else if (isBoolOperator(itr->tag)) {
        // AND, OR, NOT
        BoolOp::BopCode b_opcode = BoolOp::string2BopCode(itr->word);
        safe_next(itr);
        BoolExpr* f_bexpr = parseBoolExpr(itr);
        BoolExpr* s_bexpr;
        if (b_opcode != BoolOp::NOT) {
            safe_next(itr);
            s_bexpr = parseBoolExpr(itr);
        } else s_bexpr = nullptr; // nullptr assignment to the 2nd BoolExpr in case of a NOT clause
        return bem.makeBoolOp(b_opcode, f_bexpr, s_bexpr);

    } else if (isBoolConst(itr->tag)) {
        // TRUE, FALSE
        return bem.makeBoolConst(BoolConst::string2BoolCode(itr->word));
    } else {
        std::stringstream tmp{};
        tmp << "(ERROR (syntax): unexpected token, \"" << itr->word << "\" given )";
        throw SyntaxError(tmp.str());
    }
}


/* Recursive method used to parse a Block.
It returns a pointer to a Block object */
Block* ParseProgram::recursiveParse(std::vector<Token>::const_iterator& itr) {
    
    if (itr->tag == Token::LP) {
        safe_next(itr);  
        Block* blk = recursiveParse(itr);
        if (stmts_accumulator.size() == 0) throw SyntaxError("(ERROR (syntax): empty BLOCK statement )");
        if (itr->tag != Token::RP) {
            throw SyntaxError("(ERROR (syntax): Mismatched parenthesis )");
        }

        return blk;

    } else if (itr->tag == Token::BLOCK) {

        safe_next(itr);
        /* Counter of the statements in a block.
        If the block is empty, a runtime error is thrown */
        int cnt = 0; 
        // recursive call for each '(' to analyse the inner statements
        while (itr->tag == Token::LP) {
            recursiveParse(itr);
            cnt++;
            if (itr->tag != Token::RP) {
                throw SyntaxError("(ERROR (syntax): Mismatched parenthesis )");
            }
            safe_next(itr);
        }
        if (itr->tag != Token::RP) {
            throw SyntaxError("(ERROR (syntax): Mismatched parenthesis )");
        }
        if (cnt == 0) throw SyntaxError("(ERROR (syntax): empty block statement )");
      
        return bm.makeBlock(stmts_accumulator);


    } else if (itr->tag == Token::SET) {

        // Parsing of a SET intruction
        safe_next(itr);
        if (itr->tag == Token::VARIABLE_ID) {
            NumExpr* v = parseNumExpr(itr); // VARIABLE
            safe_next(itr);

            NumExpr* nexpr = parseNumExpr(itr); // NEXPR
            
            Statement* stmt = sm.makeSetStmt(nexpr, dynamic_cast<Variable*> (v));
            stmts_accumulator.push_back(stmt);


            recursiveParse(itr);
            safe_next(itr);
            
            if (itr->tag != Token::RP) {
                throw SyntaxError("(ERROR (syntax): Mismatched parenthesis )");
            } 

            return bm.makeBlock(stmts_accumulator);

        } else {
            std::stringstream tmp{};
            tmp << "(ERROR (syntax): VARIABLE_ID expected, \"" << itr->tag << ";" << itr->word << "\" given )";
            throw SyntaxError(tmp.str());
        } 

    } else if (itr->tag == Token::INPUT) {

        // Parsing of an INPUT instruction
        safe_next(itr);
        if (itr->tag == Token::VARIABLE_ID) {
            NumExpr* v = parseNumExpr(itr); // VARIABLE
            safe_next(itr);

            if (itr->tag != Token::RP) {
                throw SyntaxError("(ERROR (syntax): Mismatched parenthesis )");
            }

            Statement* stmt = sm.makeInputStmt((Variable*) v);
            stmts_accumulator.push_back(stmt);

            recursiveParse(itr);
        }

        return bm.makeBlock(stmts_accumulator);
            

    } else if (itr->tag == Token::PRINT) {
        // Parsing of a PRINT instruction
        safe_next(itr);
        NumExpr* v = parseNumExpr(itr);

        Statement* stmt = sm.makePrintStmt(v);
        stmts_accumulator.push_back(stmt);

        
        recursiveParse(itr);
        safe_next(itr);

        if (itr->tag != Token::RP) {
            throw SyntaxError("(ERROR (syntax): Mismatched parenthesis )");
        }

        return bm.makeBlock(stmts_accumulator);


    } else if (itr->tag == Token::IF) {
        // Parsing of an IF instruction
        safe_next(itr);
        BoolExpr* bexpr = parseBoolExpr(itr);
    
        // Temporary save of the statements accumulator
        std::vector<Statement*> tmp = stmts_accumulator;
        stmts_accumulator = {};

        /* 1st block */
        safe_next(itr);
        Block* stmt_block1 = recursiveParse(itr);

        /* 2nd block */
        stmts_accumulator = {};
        safe_next(itr);
        if (itr->tag != Token::LP) throw SyntaxError("(ERROR (syntax): missing ELSE in IF_STATEMENT )");
        Block* stmt_block2 = recursiveParse(itr);
    
        // restore of statements accumulator
        stmts_accumulator = tmp;


        Statement* stmt = sm.makeIfStmt(bexpr, stmt_block1, stmt_block2);
        stmts_accumulator.push_back(stmt);


        if (itr->tag != Token::RP) {
            throw SyntaxError("(ERROR (syntax): Mismatched parenthesis )");
        }
    
        recursiveParse(itr);
        safe_next(itr);

        return bm.makeBlock(stmts_accumulator);
        
    } else if (itr->tag == Token::WHILE) {
        // Parsing of a WHILE instruction
        safe_next(itr);
        BoolExpr* bexpr = parseBoolExpr(itr);

        // Temporary save of the statements accumulator
        std::vector<Statement*> tmp = stmts_accumulator;
        stmts_accumulator = {};
        safe_next(itr);
        Block* stmt_blk = recursiveParse(itr);

        // Restore of the statements accumulator
        stmts_accumulator = tmp;


        if (itr->tag != Token::RP) {
            throw SyntaxError("(ERROR (syntax): Mismatched parenthesis )");
        }

        Statement* stmt = sm.makeWhileStmt(bexpr, stmt_blk);
        stmts_accumulator.push_back(stmt);


        recursiveParse(itr);
        safe_next(itr);

        return bm.makeBlock(stmts_accumulator);
    } 

    return bm.makeBlock(stmts_accumulator);
}


/* Method used to perform the parsing of a Program.
It returns a pointer to a Program object */
Program* ParseProgram::parse(std::vector<Token>::const_iterator& itr, std::vector<Token>::const_iterator& end) {
    // verify if there are no read tokens (empty program)
    if (end - itr == 0) {
        /* There are TWO ways to notify if a program is empty:
            1st) returning an empy Program object and then evaluating it
            2nd) throwing a SyntaxError because the grammar doesn't allow it
        */
        //return new Program(); 
        throw SyntaxError("(ERROR (syntax): empty program )");
    } 
    Block* blk = recursiveParse(itr);
    
    // Error given if there is an overflow of tokens (other instructions outside the main block)
    if (end - itr > 1) throw SyntaxError("(ERROR (syntax): token overflow detected )");
    return new Program(blk);
}