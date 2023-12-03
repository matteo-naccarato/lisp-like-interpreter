#ifndef PROGRAM_H
#define PROGRAM_H


#include <iostream>

#include "Block.h"
#include "Statement.h"

class Visitor;

/* Class Program to store the main Block */
class Program {
/* Constants used for the flag "is_not_empty" to notify if the Program object is empty or not */
static constexpr int NOT_EMPTY_VAL = 1;
static constexpr int EMPTY_VAL = 0;

public:
    Program() : is_not_empty{ EMPTY_VAL } {}
    Program(Block* block) : blk{ block }, is_not_empty{ NOT_EMPTY_VAL } {}
    ~Program() { delete(blk); }

    Block* get_blk() const { return blk; }
    int get_is_not_empty() const { return is_not_empty; }

    void accept(Visitor* v);

private:
    Block* blk;
    int is_not_empty; // flag used to notify if the Program is empty or not
};

#endif /* PROGRAM_H */