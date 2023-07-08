#pragma once

#include <fstream>
#include <variant>
#include <string>
#include <vector>

using namespace std;

enum TokenType
{
    DEF, IDENTIFIER, RETURN, LET,

    LEFT_PAREN, RIGHT_PAREN, 
    LEFT_BRACKET, RIGHT_BRACKET, 
    LEFT_BRACE, RIGHT_BRACE,

    COMMA, COLON,
    IF, ELSE, ELSE_IF, MATCH,

    PLUS, MINUS, STAR, SLASH,
    EQUAL, EQUAL_EQUAL, BANG, BANG_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
    NUM_LIT, STR_LIT, TRUE, FALSE,

    EOF_TOK
};

struct Token 
{
    unsigned int line;
    TokenType tokentype;
    variant<int, float, string, bool> literal;
};

class Lexer
{
    public:
        vector<Token> tokens;
        unsigned int line;

        vector<Token> tokenize();

    private:
};


