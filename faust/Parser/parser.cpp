#include "parser.hpp"

std::tuple<std::vector<std::unique_ptr<Expression>>, Debug> Parser::parse()
{
    std::vector<std::unique_ptr<Expression>> expr_list;
    while (!isAtEnd())
    {
        try 
        {
            auto expr_val = expr();
            if (expr_val == nullptr)
            {
                std::cout << "NULL POINTER IN PARSER \n";
            }
            expr_list.emplace_back(std::move(expr_val));
        }
        catch (std::string msg)
        {
            debug.error(msg);
        }
    }
    debug.outputErrors();
    return std::make_tuple(std::move(expr_list), debug);
}

std::unique_ptr<Expression> Parser::expr()
{
    return equality();
}
 
std::unique_ptr<Expression> Parser::equality()
{
    auto expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        Token op = previous();
        auto right = comparison();
        return std::make_unique<BinaryExpr>(std::move(expr), op.tokentype, std::move(right));
    }
    return expr;
}
 
std::unique_ptr<Expression> Parser::comparison()
{
    auto expr = term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
    {
        Token op = previous();
        auto right = term();
        return std::make_unique<BinaryExpr>(std::move(expr), op.tokentype, std::move(right));
    }
    return expr;
}
 
std::unique_ptr<Expression> Parser::term()
{
    auto expr = factor();
    while (match({TokenType::MINUS, TokenType::PLUS}))
    {
        Token op = previous();
        auto right = factor();
        if (right == nullptr)
            std::cout << "RIGHT NULL\n";
        return std::make_unique<BinaryExpr>(std::move(expr), op.tokentype, std::move(right));
    }
    return expr;
}
 
std::unique_ptr<Expression> Parser::factor()
{
    auto expr = unary();
    while (match({TokenType::SLASH, TokenType::STAR}))
    {
        Token op = previous();
        auto right = unary();
        return std::make_unique<BinaryExpr>(std::move(expr), op.tokentype, std::move(right));
    }
    return expr;
}
  
std::unique_ptr<Expression> Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS}))
    {
        Token op = previous();
        auto right = unary();
        return std::make_unique<UnaryExpr>(op.tokentype, std::move(right));
    }
    return primary();
}
 
std::unique_ptr<Expression> Parser::primary()
{
    if (match({TokenType::TRUE}))
        return std::make_unique<BoolExpr>(true);
    if (match({TokenType::FALSE}))
        return std::make_unique<BoolExpr>(false);
    if (match({TokenType::INT_LIT, TokenType::FLOAT_LIT, TokenType::STR_LIT}))
        switch (previous().tokentype)
        {
            case TokenType::INT_LIT:
                return std::make_unique<IntExpr>(std::get<int>(previous().literal));
            case TokenType::FLOAT_LIT:
                return std::make_unique<FloatExpr>(std::get<float>(previous().literal));
            case TokenType::STR_LIT:
                return std::make_unique<StringExpr>(std::get<std::string>(previous().literal));
        }
    if (match({TokenType::LEFT_PAREN}))
    {
        std::unique_ptr<Expression> expr_call = expr();
        try 
        {
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
            return std::make_unique<Grouping>(std::move(expr_call));
        }
        catch (std::string msg)
        {
            throw msg;
        }
    }
}

Token Parser::advance()
{
    if (!isAtEnd())
        current++;
    return previous();
}
 
Token Parser::peek()
{
    return tokens[current];
}
 
Token Parser::previous()
{
    return tokens[current - 1];
}

Token Parser::consume(TokenType token, std::string msg)
{
    if (check(token))
        return advance();
    throw msg;
}

bool Parser::isAtEnd()
{
    return peek().tokentype == TokenType::EOF_TOK;
}
 
bool Parser::check(TokenType token)
{
    if (isAtEnd())
        return false;
    return peek().tokentype == token;
}
 
bool Parser::match(std::vector<TokenType> types)
{
    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        if (check(types[i]))
        {
            advance();
            return true;
        }
    }
    return false;
}


