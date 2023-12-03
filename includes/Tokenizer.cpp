#include <string>
#include <sstream>

#include "Tokenizer.h"
#include "Exceptions.h"


// Method for the "tokenize" phase passing a file to read from and a std::vector in which to store the read tokens as arguments 
void Tokenizer::tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens) {

    int line_number = 1; // counter used as indicator of the line number
    int isonlyalpha = 1; // flag used to verify the correctness of the variables which characters have to be alphabetic
    int zero_as_first = 0; // useful flag to notify if a number starts with a 0 (there can be no other digits after the 0)
    int neg = 0; // flag used to read tokens representing negative numbers 
    char ch;
    ch = inputFile.get();

    while (!inputFile.eof()) {
        if (ch == '\n') line_number++;
        if (std::isspace(ch)) { // 'if' useful to jump useless spaces 
            ch = inputFile.get();
            continue;
        }
        if (ch == '(') inputTokens.push_back(Token{ Token::LP, Token::id2word[Token::LP] });
        else if (ch == ')') inputTokens.push_back(Token{ Token::RP, Token::id2word[Token::RP] });
        else if (std::isalpha(ch)) {
            std::stringstream tmp{};
            tmp << ch;
            do {
                ch = inputFile.get();
                if (std::isalpha(ch)) tmp << ch; 
                else if (ch != ' ' && ch != '(' && ch != ')' && ch != '\n') isonlyalpha = 0; 
            } while (std::isalpha(ch));
            
            // possible tokens 
            if (tmp.str() == Token::id2word[Token::BLOCK]) inputTokens.push_back(Token{ Token::BLOCK, Token::id2word[Token::BLOCK] });
            else if (tmp.str() == Token::id2word[Token::SET]) inputTokens.push_back(Token{ Token::SET, Token::id2word[Token::SET] });
            else if (tmp.str() == Token::id2word[Token::PRINT]) inputTokens.push_back(Token{ Token::PRINT, Token::id2word[Token::PRINT] });
            else if (tmp.str() == Token::id2word[Token::INPUT]) inputTokens.push_back(Token{ Token::INPUT, Token::id2word[Token::INPUT] });
            else if (tmp.str() == Token::id2word[Token::IF]) inputTokens.push_back(Token{ Token::IF, Token::id2word[Token::IF] });
            else if (tmp.str() == Token::id2word[Token::WHILE]) inputTokens.push_back(Token{ Token::WHILE, Token::id2word[Token::WHILE] });
            else if (tmp.str() == Token::id2word[Token::ADD]) inputTokens.push_back(Token{ Token::ADD, Token::id2word[Token::ADD] });
            else if (tmp.str() == Token::id2word[Token::SUB]) inputTokens.push_back(Token{ Token::SUB, Token::id2word[Token::SUB] });
            else if (tmp.str() == Token::id2word[Token::MUL]) inputTokens.push_back(Token{ Token::MUL, Token::id2word[Token::MUL] });
            else if (tmp.str() == Token::id2word[Token::DIV]) inputTokens.push_back(Token{ Token::DIV, Token::id2word[Token::DIV] });
            else if (tmp.str() == Token::id2word[Token::LT]) inputTokens.push_back(Token{ Token::LT, Token::id2word[Token::LT] });
            else if (tmp.str() == Token::id2word[Token::GT]) inputTokens.push_back(Token{ Token::GT, Token::id2word[Token::GT] });
            else if (tmp.str() == Token::id2word[Token::EQ]) inputTokens.push_back(Token{ Token::EQ, Token::id2word[Token::EQ] });
            else if (tmp.str() == Token::id2word[Token::AND]) inputTokens.push_back(Token{ Token::AND, Token::id2word[Token::AND] });
            else if (tmp.str() == Token::id2word[Token::OR]) inputTokens.push_back(Token{ Token::OR, Token::id2word[Token::OR] });
            else if (tmp.str() == Token::id2word[Token::NOT]) inputTokens.push_back(Token{ Token::NOT, Token::id2word[Token::NOT] });
            else if (tmp.str() == Token::id2word[Token::TRUE]) inputTokens.push_back(Token{ Token::TRUE, Token::id2word[Token::TRUE] });
            else if (tmp.str() == Token::id2word[Token::FALSE]) inputTokens.push_back(Token{ Token::FALSE, Token::id2word[Token::FALSE] });
            else {
                /* VARIABLE expr */
                if (isonlyalpha) {
                    inputTokens.push_back(Token{ Token::VARIABLE_ID, tmp.str() });
                } else {
                    std::stringstream tmp{};
                    tmp << "(ERROR (lexical) in tokenizer: non-alpha character in VARIABLE_ID at line " << line_number << " )";
                    throw LexicalError(tmp.str());   
                }
            }
            continue;
        }
        else if (ch == '-') {
            neg = 1; // flag identifying negative numbers set to 1 
        }
        else if (std::isdigit(ch)) { // number interpretation
            std::stringstream tmp{};
            tmp << ch;
            zero_as_first = (ch == '0'); // flag value set to 1 if the 1st character is '0', otherwise is se to 0 
            do {
                ch = inputFile.get();
                if (std::isdigit(ch) && zero_as_first) throw LexicalError("(ERROR (lexical): numbers can't have a sequence of zeros as first digits )");
                if (std::isdigit(ch)) tmp << ch;
            } while (std::isdigit(ch));
            if (ch != ' ' && ch != '(' && ch != ')' && ch != ' ' && ch != '\n') { // verify that numbers are made only by numbers and not by other characters
                std::stringstream tmp{};
                tmp << "(ERROR (lexical): numbers have to be made of only NUMBERS; '" << ch << "' given at line number " << line_number << " )";
                throw LexicalError(tmp.str());
            } 
            inputTokens.push_back(Token{ Token::NUM, (((neg)?"-":"") + tmp.str()) });
            neg = 0; // negative numbers flag reset
            continue;

        } else {
            // Not recognize symbol during tokenize phase
            std::stringstream tmp{};
            tmp << "(ERROR (lexical) in tokenizer: Stray character '" << ch << "' in input at line " << line_number << " )";
            throw LexicalError(tmp.str());
        }
        isonlyalpha = 1; // reset flag used to identify if a string is made only by alphabetic characters
        ch = inputFile.get();
    }
} 