#ifndef BLOCK_H
#define BLOCK_H


#include <iostream>

#include <vector>

#include "Statement.h"

class Visitor;

/* Class Block where the only attribute is a std::vector of pointers to Statement objects */
class Block {
public:
    Block() = default;
    Block(const Block& other) = default;
    Block(std::vector<Statement*> s) : stmts{ s } {} 
    ~Block() = default;
    
    std::vector<Statement*> get_stmts() const { return stmts; } 
    
    void accept(Visitor* v);

private:
    std::vector<Statement*> stmts;
};


#endif /* BLOCK_H */