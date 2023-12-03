#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>

#include "includes/Token.h"
#include "includes/Tokenizer.h"
#include "includes/Exceptions.h"
#include "includes/Parser.h"
#include "includes/Visitor.h"


int main(int argc, char* argv[]) {

    /* Check number of params */
    if (argc < 2) {
        std::cerr << "(ERROR: File not specified! )" << std::endl;
        std::cerr << "USAGE: " << argv[0] << " <FILE_PATH>" << std::endl;
        return EXIT_FAILURE;
    }


    /* Tokenizer */
    Tokenizer tokenize;
    std::vector<Token> inputTokens;
    std::ifstream inputFile;
    try {
        inputFile.open(argv[1]);
        if (inputFile.fail()) {
            std::cerr << "(ERROR: fail to open file \"" << argv[1] << "\" )" <<  std::endl;
            return EXIT_FAILURE;
        }
    } catch (std::exception& exc) {
        std::cerr << "(ERROR: it is not possible to open file \"" << argv[1] << "\" )" << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }


    try {
        inputTokens = std::move(tokenize(inputFile));
        inputFile.close();
    } catch (LexicalError& le) {
        std::cerr << le.what() << std::endl;
        return EXIT_FAILURE;
    } catch (std::exception& exc) {
        std::cerr << "(ERROR: it is not possible to read from (" << argv[1] << "\" )" << std::endl;
        std::cerr << exc.what();
    }


    // PARSING
    BlockManager block_manager;
    StatementManager stmt_manager;
    NumExprManager numexpr_manager;
    BoolExprManager bexpr_manager;
    ParseProgram parse{ block_manager, stmt_manager, numexpr_manager, bexpr_manager }; // function object per la fase di parsing


    // EVALUATION
    Program* prg;
    EvaluationVisitor* v = new EvaluationVisitor();
    
    try {
        prg = parse(inputTokens);
        prg->accept(v);
        
        delete(prg);
        delete(v); 

    } catch (LexicalError& le) {
        std::cerr << le.what() << std::endl;
        delete(prg);
        delete(v);
        return EXIT_FAILURE;

    } catch (SyntaxError& pe) {
        std::cerr << pe.what() << std::endl;
        delete(v);
        return EXIT_FAILURE;

    } catch (SemanticError& se) {
        std::cerr << se.what() << std::endl;
        delete(prg);
        delete(v);
        return EXIT_FAILURE;
        
    } catch (std::exception& exc) {
        std::cerr << "(ERROR: generic error )" << std::endl;
        std::cerr << exc.what() << std::endl;
        delete(prg);
        delete(v);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}