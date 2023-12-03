#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <fstream>

#include "Token.h"

/* Function object to perform tokenize phase given a file in input.
It returns a std::vector containing the read tokens */
class Tokenizer {
public:
    std::vector<Token> operator()(std::ifstream& inputFile) {
        std::vector<Token> inputTokens;
        tokenizeInputFile(inputFile, inputTokens);
        return inputTokens;
    }

private:
    void tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens);
};

#endif /* TOKENIZER_H */