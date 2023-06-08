#pragma once

#include <vector>
#include <string>
#include <variant>

enum TokenType 
{
    FN = 1,
    RETURN = 2,

    INT = 3,
    FLOAT = 4,
    STRING = 5,
    CHAR = 6,
    BOOL = 7,

    TRUE = 8,
    FALSE = 9,

    PLUS = 10,
    MINUS = 11,
    STAR = 12,
    SLASH = 13,
    
    INT_LIT = 14,
    STR_LIT = 15,
    CHAR_LIT = 16,
    FLOAT_LIT = 17,

    SEMICOLON = 18,
    
    EQUAL = 19,
    EQUAL_EQUAL = 20,
    BANG = 21,
    BANG_EQUAL = 22,
    GREATER = 23,
    GREATER_EQUAL = 24,
    LESS = 25,
    LESS_EQUAL = 26,

    EOF_TOK = 0,
};

struct Token
{   
    TokenType tokentype;
    unsigned int line;
    std::variant<int, float, std::string> val;
};

class Lexer 
{
    public:
        std::vector<Token> tokens;
        unsigned int line;

        std::vector<Token> Tokenize();
        void Print_Tokens();

    private:
        bool writing;

        void Match_Token(std::string& str);
        void Add_Token(TokenType token);
        void Add_Token(TokenType token, int num);
        void Add_Token(TokenType token, float num);
        void Add_Token(TokenType token, std::string str);

};
