#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <sstream>

// struct used to identify possible Tokens
struct Token {

    static constexpr int LP = 0; // (
    static constexpr int RP = 1; // )
    static constexpr int NUM = 2;
    static constexpr int BLOCK = 3;
    static constexpr int SET = 4;
    static constexpr int PRINT = 5;
    static constexpr int INPUT = 6;
    static constexpr int IF = 7;
    static constexpr int WHILE = 8;
    static constexpr int ADD = 9;
    static constexpr int SUB = 10;
    static constexpr int MUL = 11;
    static constexpr int DIV = 12;
    static constexpr int LT = 13;
    static constexpr int GT = 14;
    static constexpr int EQ = 15;
    static constexpr int AND = 16;
    static constexpr int OR = 17;
    static constexpr int NOT = 18;
    static constexpr int TRUE = 19;
    static constexpr int FALSE = 20;
    static constexpr int VARIABLE_ID = 21;

    static constexpr const char* id2word[] {
        "(", ")", "NUM", "BLOCK", "SET", "PRINT", "INPUT", "IF", "WHILE", "ADD", "SUB", "MUL", "DIV", "LT", "GT", "EQ", "AND", "OR", "NOT", "TRUE", "FALSE", "VARIABLE_ID"
    };

    Token(int t, const char* w) : tag{ t }, word{ w } {};
    Token(int t, std::string w) : tag { t }, word { w } {};

    int tag;
    std::string word;
};

std::ostream& operator<<(std::ostream& os, const Token& t);

#endif /* TOKEN_H */