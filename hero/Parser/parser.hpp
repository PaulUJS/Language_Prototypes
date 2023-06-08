#pragma once

#include "../Lexer/lexer.hpp"
#include "../AST/ast.hpp"

class Parser
{
    public:
        Parser(std::vector<Token> tokens)
            : tokens(tokens) {}
        ~Parser() {}

        std::vector<std::unique_ptr<Expression>> Parse();

    private:
        std::vector<Token> tokens;
        unsigned int index;

        std::unique_ptr<Expression> Parse_Expression();
        std::unique_ptr<Expression> Parse_Equality();
        std::unique_ptr<Expression> Parse_Comparison();
        std::unique_ptr<Expression> Parse_Term();
        std::unique_ptr<Expression> Parse_Factor(); 
        std::unique_ptr<Expression> Parse_Unary();
        std::unique_ptr<Expression> Parse_Primary();

        bool Match(std::vector<TokenType> tokens);
        bool At_End();
        bool Check(TokenType token);

        Token Peek();
        Token Previous();
        Token Advance();
};
