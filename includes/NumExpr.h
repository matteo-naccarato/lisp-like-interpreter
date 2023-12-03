#ifndef NUM_EXPR_H
#define NUM_EXPR_H

#include <string>
#include <vector>

#include "Token.h"

class Visitor;
class Operator;


/* Superclass used to identify the numerical expression */
class NumExpr {
public:
    virtual ~NumExpr() {};
    virtual void accept(Visitor* v) = 0;
};

/* Class that extends NumExpr to represent the arithmetic operations between two NumExpr operands */
class Operator : public NumExpr {
public:
    /* Enumeration to identify the four arithmetic operations (NULL_VAL is useful to identify a invalid value) */
    enum OpCode { ADD, SUB, MUL, DIV, NULL_VAL };

    Operator(OpCode o, NumExpr* fop, NumExpr* sop) :
        op{ o }, first{ fop }, second{ sop } {}
    Operator(const Operator& other) = default;
    ~Operator() = default;
    Operator& operator=(const Operator& other) = default;

    OpCode getOp() const { return op; }
    NumExpr* getFirst() const { return first; }
    NumExpr* getSecond() const { return second; }

    void accept(Visitor* v) override;


    static OpCode string2OpCode(std::string s) {
        if (s == Token::id2word[Token::ADD]) return ADD;
        if (s == Token::id2word[Token::SUB]) return SUB;
        if (s == Token::id2word[Token::MUL]) return MUL;
        if (s == Token::id2word[Token::DIV]) return DIV;
        return NULL_VAL;
    }

    static std::string opCode2String(OpCode op) {
        std::string s;
        switch (op) {
            case ADD: s = Token::id2word[Token::ADD]; break;
            case SUB: s = Token::id2word[Token::SUB]; break;
            case MUL: s = Token::id2word[Token::MUL]; break;
            case DIV: s = Token::id2word[Token::DIV]; break;
            default: s = " ";
        }
        return s;
    }

private:
    OpCode op;
    NumExpr* first;
    NumExpr* second;
};


/* Class that extends NumExpr to represent integer numerical values in 64bit registers (c++ int64_t) */
class Number : public NumExpr {
public:
    Number(int64_t v) : value{ v } {};
    Number(const Number& other) = default;
    ~Number() = default;
    Number& operator=(const Number& other) = default;

    int64_t get_value() const { return value; }

    void accept(Visitor* v) override;

private:
    int64_t value;
};


/* Class that extends NumExpr to represent variables */
class Variable : public NumExpr { 

// default value for variables created with just the std::string parameter (VARIABLE_ID)
static constexpr int64_t DEFAULT_VAL = 0;

public:
    Variable(std::string i) : Variable(i, DEFAULT_VAL) {}
    Variable(std::string i, int64_t v) : id{ i }, value{ v } {}
    Variable(const Variable& other) = default;
    ~Variable() = default;
    Variable& operator=(const Variable& other) = default;

    std::string get_id() const { return id; }
    int64_t get_value() const { return value; }
    void set_value(int64_t val) { value = val; }

    void accept(Visitor* v) override;

private:
    std::string id;
    int64_t value;
};


#endif /* NUM_EXPR_H */