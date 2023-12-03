#ifndef BOOL_EXPR_H
#define BOOL_EXPR_H

#include <string>

#include "Token.h"
#include "NumExpr.h"


class Visitor;

/* BoolExpr superclass used to identify the boolean expressions */
class BoolExpr {
public:
    virtual ~BoolExpr() {};
    virtual void accept(Visitor* v) = 0;        
};


/* Class that extends BoolExpr to create objects reprenting the boolean constants TRUE and FALSE */
class BoolConst : public BoolExpr {
public:
    /* Enumeration used to identify the two boolean constants (the value NULL_VAL has the utility to identify a invalid value) */
    enum BoolCode { TRUE, FALSE, NULL_VAL };

    BoolConst(BoolCode bcode) : bconst{ bcode } {};
    ~BoolConst() = default;
    BoolConst(const BoolConst& other) = default;
    BoolConst& operator=(const BoolConst& other) = default;

    BoolCode get_bconst() const { return bconst; }

    void accept(Visitor* v) override;

    static BoolCode string2BoolCode(std::string s) {
        if (s == Token::id2word[Token::TRUE]) return TRUE;
        if (s == Token::id2word[Token::FALSE]) return FALSE;
        return NULL_VAL;
    } 

    static std::string boolCode2String(BoolCode bcode) {
        std::string s;
        switch (bcode) {
            case TRUE: s = Token::id2word[Token::TRUE]; break;
            case FALSE: s = Token::id2word[Token::FALSE]; break;
            default: s = " ";
        }
        return s;
    }

private:
    BoolCode bconst;
};



/* Class that extends BoolExpr to identify boolean operators AND, OR and NOT (NULL_VAL is useful to identify an invalid value) */
class BoolOp : public BoolExpr {
public:
    enum BopCode { AND, OR, NOT, NULL_VAL };

    BoolOp(BopCode b, BoolExpr* fbexpr, BoolExpr* sbexpr) : b_opcode{ b }, f_bexpr{ fbexpr }, s_bexpr{ sbexpr } {};
    ~BoolOp() = default; 
    BoolOp(const BoolOp& other) = default;
    BoolOp& operator=(const BoolOp& other) = default;

    BopCode get_b_opcode() const { return b_opcode; }
    BoolExpr* get_f_bexpr() const { return f_bexpr; }
    BoolExpr* get_s_bexpr() const { return s_bexpr; }

    void accept(Visitor* v) override;


    static BopCode string2BopCode(std::string s) {
        if (s == Token::id2word[Token::AND]) return AND;
        if (s == Token::id2word[Token::OR]) return OR;
        if (s == Token::id2word[Token::NOT]) return NOT;
        return NULL_VAL;
    }

    static std::string bopCode2String(BopCode bcode) {
        std::string s;
        switch(bcode) {
            case AND: s = Token::id2word[Token::AND]; break;
            case OR: s = Token::id2word[Token::OR]; break;
            case NOT: s = Token::id2word[Token::NOT]; break;
            default: s = " ";
        }
        return s;
    }
    

private:
    BopCode b_opcode; 
    BoolExpr* f_bexpr;
    BoolExpr* s_bexpr;
};



/* Class that extends BoolExpr to identify relational operators LT, GT and EQ (NULL_VAL is useful to identify an invalid value) */
class RelOp : public BoolExpr {
public:
    enum RelOpCode { LT, GT, EQ, NULL_VAL };

    RelOp(RelOpCode r, NumExpr* f_nexpr, NumExpr* s_nexpr) : r_opcode{ r }, first_nexpr{ f_nexpr }, second_nexpr{ s_nexpr } {};
    ~RelOp() = default;
    RelOp(const RelOp& other) = default;
    RelOp& operator=(const RelOp& other) = default;

    RelOpCode get_r_opcode() const { return r_opcode; }
    NumExpr* get_first_nexpr() const { return first_nexpr; }
    NumExpr* get_second_nexpr() const { return second_nexpr; }

    void accept(Visitor* v) override;

    static RelOpCode string2RelOpCode(std::string s) {
        if (s == Token::id2word[Token::LT]) return LT;
        if (s == Token::id2word[Token::GT]) return GT;
        if (s == Token::id2word[Token::EQ]) return EQ;
        return NULL_VAL;
    }

    static std::string relOpCode2String(RelOpCode r_opcode) {
        std::string s;
        switch(r_opcode) {
            case LT: s = Token::id2word[Token::LT]; break;
            case GT: s = Token::id2word[Token::GT]; break;
            case EQ: s = Token::id2word[Token::EQ]; break;
            default: s = " ";
        }
        return s;
    }

private:
    RelOpCode r_opcode;
    NumExpr* first_nexpr;
    NumExpr* second_nexpr;
};


#endif /* BOOL_EXPR_H */