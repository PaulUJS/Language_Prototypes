#include "lexer.hpp"

#include <iostream>

std::vector<Token> Lexer::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }
    tokens.push_back(Token { TokenType::EOF_TOK, line, false });
    return tokens;
}

void Lexer::scanToken()
{
    char ch = advance();
    switch (ch)
    {
        case '(':
            addToken(TokenType::LEFT_PAREN);
            break;
        case ')':
            addToken(TokenType::RIGHT_PAREN);
            break;
        case '[':
            addToken(TokenType::LEFT_BRACKET);
            break;
        case ']':
            addToken(TokenType::RIGHT_BRACKET);
            break;
        case '{':
            addToken(TokenType::LEFT_BRACE);
            break;
        case '}':
            addToken(TokenType::RIGHT_BRACE);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case ' ':
            break;
        case '\r':
            break;
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '"':
            lexString();
            break;
        case '*':
            addToken(TokenType::STAR);
            break;
        case '+':
            addToken(match('+') ? TokenType::PLUS_PLUS : TokenType::PLUS);
            break;
        case '-':
            addToken(match('>') ? TokenType::ARROW : TokenType::MINUS);
            break;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case ':':
            addToken(match(':') ? TokenType::COLON_COLON : TokenType::COLON);
            break;
        case '/':
            if (match('/'))
            {
                while (peek() != '\n' && !isAtEnd())
                    advance();
            }
            else 
            {
                addToken(TokenType::SLASH);
            }
            break;
        default:
            if (isdigit(ch))
                lexNumber();
            else if (isAlpha(ch))
                identifier();
            else 
                debug.error((std::string)"Unidentified character");
            break;
    }
}

void Lexer::addToken(TokenType token)
{
    tokens.push_back(Token { token, line, false });
}

void Lexer::addToken(TokenType token, int val)
{
    tokens.push_back(Token { token, line, val });
}

void Lexer::addToken(TokenType token, float val)
{
    tokens.push_back(Token { token, line, val });
}

void Lexer::addToken(TokenType token, bool val)
{
    tokens.push_back(Token { token, line, val });
}

void Lexer::addToken(TokenType token, std::string val)
{
    tokens.push_back(Token { token, line, val });
}

void Lexer::identifier()
{
    while (isAlphaNumeric(peek()))
        advance();
    std::string text = source.substr(start, current - start);
    if (keywords.find(text) == keywords.end())
    {
        addToken(TokenType::IDENTIFIER, text);
    }
    else 
        addToken(keywords.at(text));
}

void Lexer::lexNumber()
{
    bool isF = false;
    while (isdigit(peek()))
        advance();
    if (peek() == '.' && isdigit(peekNext())) 
    {
        isF = true;
        advance();
        while (isdigit(peek()))
            advance();
    }
    std::string sub = source.substr(start, current - start);
    if (isF)
        addToken(TokenType::FLOAT_LIT, stof(sub));
    else if (!isF)
        addToken(TokenType::INT_LIT, stoi(sub));
}

void Lexer::lexString()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            line++;
        advance();
    }
    if (isAtEnd())
    {
        debug.error((std::string)"Unterminated String");
        return;
    }
    advance();
    std::string val = source.substr(start + 1, (current - start) - 2);
    addToken(TokenType::STR_LIT, val);
}

char Lexer::advance()
{
    return source[current++];
}

char Lexer::peek()
{
    if (isAtEnd())
        return '\0';
    return source[current];
}

char Lexer::peekNext()
{
    if (current + 1 >= source.size())
        return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected)
{
    if (isAtEnd()) 
        return false;
    if (source[current] != expected)
        return false;
    current++;
    return true;
}

bool Lexer::isAlpha(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

bool Lexer::isAlphaNumeric(char ch)
{
    return isAlpha(ch) || isdigit(ch);
}

bool Lexer::isAtEnd()
{
    return current >= source.size();
}
