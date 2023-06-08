#include "parser.hpp"

#include <variant>

std::vector<std::unique_ptr<Expression>> Parser::Parse()
{
    std::cout << "\n" << "PARSING \n" << "____________________ \n" << "\n";
    std::vector<std::unique_ptr<Expression>> expr;
    while (!At_End())
    {
       expr.push_back(Parse_Expression()); 
    }
    return expr;
}

std::unique_ptr<Expression> Parser::Parse_Expression()
{
    return Parse_Equality();
}

std::unique_ptr<Expression> Parser::Parse_Equality()
{
    auto expr = Parse_Comparison();
    while (Match({BANG_EQUAL, EQUAL_EQUAL}))
    {
        TokenType op = Previous().tokentype;
        auto right = Parse_Comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expression> Parser::Parse_Comparison()
{
    auto expr = Parse_Term();
    while (Match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL}))
    {
        TokenType op = Previous().tokentype;
        auto right = Parse_Term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expression> Parser::Parse_Term()
{
    auto expr = Parse_Factor();
    while (Match({MINUS, PLUS}))
    {
        TokenType op = Previous().tokentype;
        auto right = Parse_Factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expression> Parser::Parse_Factor()
{
    auto expr = Parse_Unary();
    while (Match({SLASH, STAR}))
    {
        TokenType op = Previous().tokentype;
        auto right = Parse_Unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expression> Parser::Parse_Unary()
{
    if (Match({BANG, MINUS}))
    {
        TokenType op = Previous().tokentype;
        auto right = Parse_Unary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }
    return Parse_Primary();
}

std::unique_ptr<Expression> Parser::Parse_Primary()
{
    if (Match({INT_LIT}))
        return std::make_unique<IntExpr>(std::get<int>(Previous().val));
    else if (Match({FLOAT_LIT}))
        return std::make_unique<FloatExpr>(std::get<float>(Previous().val));
    else if (Match({STR_LIT}))
        return std::make_unique<StringExpr>(std::get<std::string>(Previous().val));
    else if (Match({TRUE}))
        return std::make_unique<BoolExpr>(true);
    else if (Match({FALSE}))
        return std::make_unique<BoolExpr>(false);
    else 
        return std::make_unique<NullExpr>();
}

Token Parser::Previous()
{
    return tokens[index - 1];
}

Token Parser::Peek()
{
    return tokens[index];
}

Token Parser::Advance()
{
    if (!At_End())
        index++;
    return Previous();
}

bool Parser::Match(std::vector<TokenType> tokens)
{
    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        if (Check(tokens[i]))
        {
            Advance();
            return true;
        }
    }
    return false;
}

bool Parser::Check(TokenType token)
{
    if (At_End())
        return false;
    return Peek().tokentype == token;
}

bool Parser::At_End()
{
    return Peek().tokentype == EOF_TOK;
}
