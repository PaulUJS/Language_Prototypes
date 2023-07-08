#pragma once

#include "../Debug/debug.hpp"

#include <string>
#include <vector>
#include <variant>
#include <unordered_map>

enum class TokenType
{
    DEF, RETURN, IF, ELSE, FOR, WHILE, END, AND, OR,
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET, LEFT_BRACE, RIGHT_BRACE,
    PLUS, MINUS, STAR, SLASH,
    EQUAL, EQUAL_EQUAL, BANG, BANG_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
    ARROW, COLON, COLON_COLON, IDENTIFIER, PLUS_PLUS, COMMA, 
    INT_LIT, FLOAT_LIT, STR_LIT, TRUE, FALSE,
    INT, FLOAT, STRING, BOOL,
    EOF_TOK,
};

struct Token 
{
    TokenType tokentype;
    unsigned int line;
    std::variant<int, float, std::string, bool> literal;
};

class Lexer
{
    public:
        Debug debug;
        Lexer(const std::string& source)
            : source(source), start(0), current(0), line(1) {}
        std::vector<Token> scanTokens();        

    private:
        std::vector<Token> tokens;
        unsigned int start, current, line;
        const std::string& source;
        std::unordered_map<std::string, TokenType> keywords = 
        {
            {"def", TokenType::DEF},
            {"return", TokenType::RETURN},
            {"if", TokenType::IF},
            {"else", TokenType::ELSE},
            {"end", TokenType::END},
            {"and", TokenType::AND},
            {"or", TokenType::OR},
            {"for", TokenType::FOR},
            {"while", TokenType::WHILE},
            {"true", TokenType::TRUE},
            {"false", TokenType::FALSE},
            {"int", TokenType::INT},
            {"float", TokenType::FLOAT},
            {"string", TokenType::STRING},
            {"bool", TokenType::BOOL}
        };

        void scanToken();
        void addToken(TokenType token);
        void addToken(TokenType token, int val);
        void addToken(TokenType token, float val);
        void addToken(TokenType token, bool val);
        void addToken(TokenType token, std::string val);
        void lexNumber();
        void lexString();
        void identifier();

        bool match(char expected);
        bool isAtEnd();
        bool isAlpha(char ch);
        bool isAlphaNumeric(char ch);
        
        char peek();
        char peekNext();
        char advance();
};
