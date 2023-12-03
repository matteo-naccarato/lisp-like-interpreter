#ifndef PARSER_H
#define PARSER_H


#include <string>
#include <vector>

#include "Token.h"
#include "Program.h"
#include "Block.h"
#include "BlockManager.h"
#include "StatementManager.h"
#include "NumExprManager.h"
#include "BoolExprManager.h"
#include "Exceptions.h"


/* Function object used to manage the parsing of a token stream in a Program object */
class ParseProgram { 
public:
    ParseProgram(BlockManager& block_m, StatementManager& stmt_m, NumExprManager& nexpr_m, BoolExprManager& bexpr_m) : bm{ block_m }, sm{ stmt_m }, nem{ nexpr_m }, bem{ bexpr_m }, stmts_accumulator{ } {};
    ~ParseProgram() = default;  
    
    // Deletion of default constructor because specifying BlockManager, StatementManager, NumExprManager and BoolExprManager as parameters is required
    ParseProgram() = delete;
    // Deletion of copy constructor and the assignment operator to avoid pointers ownership errors
    ParseProgram(const ParseProgram& other) = delete;
    ParseProgram& operator=(const ParseProgram& other) = delete;

    Program* operator()(const std::vector<Token>& tokenStream) {
        auto tokenItr = tokenStream.begin();
        auto tokenEnd = tokenStream.end();
        Program* prg =  parse(tokenItr, tokenEnd);
        return prg;
    }


private:
    std::vector<Token>::const_iterator streamEnd;
    BlockManager& bm;
    StatementManager& sm;
    NumExprManager& nem;
    BoolExprManager& bem;
    std::vector<Statement*> stmts_accumulator; // the deallocation of this std::vector<Statement*> is performed by the BlockManager destructor 
    
    Program* parse(std::vector<Token>::const_iterator& tokenItr, std::vector<Token>::const_iterator& tokenEnd);
    Block* recursiveParse(std::vector<Token>::const_iterator& tokenItr);
    NumExpr* parseNumExpr(std::vector<Token>::const_iterator& tokenItr);
    BoolExpr* parseBoolExpr(std::vector<Token>::const_iterator& tokenItr);

    // util methods used to evaluate the identity of characters or TOKEN_ID (int)
    bool isOperator(int t) const { return t == Token::ADD || t == Token::SUB || t == Token::MUL || t == Token::DIV; }
    bool isRelOp(int b) const { return b == Token::LT || b == Token::GT || b == Token::EQ; }
    bool isBoolConst(int b) const { return b == Token::TRUE || b == Token::FALSE; }
    bool isBoolOperator(int r) const { return r == Token::AND || r == Token::OR || r == Token::NOT; }
 
    // method used to progress an iterator passed as parameter
    void safe_next(std::vector<Token>::const_iterator& itr) {
        if (itr == streamEnd) {
            throw SyntaxError("(ERROR (syntax): unexpected end of input )");
        }
        ++itr;
    }
};

#endif /* PARSER_H */ 