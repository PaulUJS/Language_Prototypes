#pragma once

#include "../Debug/debug.hpp"
#include "../Lexer/lexer.hpp"
#include "../Ast/ast.hpp"

#include <tuple>

class Parser
{
    public:
        Parser(std::vector<Token> tokens, Debug debug)
            : tokens(tokens), debug(debug) {}
        std::tuple<std::vector<std::unique_ptr<Expression>>, Debug> parse();
    private:
        unsigned int current;
        Debug debug;
        std::vector<Token> tokens;

        std::unique_ptr<Expression> expr();
        std::unique_ptr<Expression> equality();
        std::unique_ptr<Expression> comparison();
        std::unique_ptr<Expression> term();
        std::unique_ptr<Expression> factor();
        std::unique_ptr<Expression> unary();
        std::unique_ptr<Expression> primary();

        Token advance();
        Token peek();
        Token previous();
        Token consume(TokenType token, std::string msg);

        bool isAtEnd();
        bool check(TokenType token);
        bool match(std::vector<TokenType> tokens);
};
