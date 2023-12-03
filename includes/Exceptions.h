#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>


/* Structures dedicated to classify possible errors during the execution of the interpreter */
struct LexicalError : std::runtime_error {
    LexicalError(const char* msg) : std::runtime_error(msg) {}
    LexicalError(std::string msg) : std::runtime_error(msg.c_str()) {}
};

struct SyntaxError : std::runtime_error {
    SyntaxError(const char* msg) : std::runtime_error(msg) {}
    SyntaxError(std::string msg) : std::runtime_error(msg.c_str()) {}
};

struct SemanticError : std::runtime_error {
   SemanticError(const char* msg) : std::runtime_error(msg) {}
   SemanticError(std::string msg) : std::runtime_error(msg.c_str()) {}
};


#endif /* EXCEPTIONS_H */