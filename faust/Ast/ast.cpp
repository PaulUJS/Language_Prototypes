#include "ast.hpp"

std::tuple<std::variant<int, float, std::string, bool>, TokenType> BinaryExpr::evaluate()
{
    if (left != nullptr && right != nullptr)
    {
        auto [lv, lt] = left->evaluate();
        auto [rv, rt] = right->evaluate();
        if (lt == rt)
        {
            switch (lt)
            {
                case TokenType::INT_LIT:
                    return evalInt(std::get<int>(lv), std::get<int>(rv));
                case TokenType::FLOAT_LIT:
                    return evalFloat(std::get<float>(lv), std::get<float>(rv));
                case TokenType::STR_LIT:
                    return evalString(std::get<std::string>(lv), std::get<std::string>(rv));
                case TokenType::BOOL:
                    throw "Cannot do operations of type BOOL";
            }
        }
        else 
        {
            throw (std::string)"Cannot do operation on non-matching types";
        }
    }
    else 
    {
        throw (std::string)"ERROR NULLPTR";
    }
}

std::tuple<std::variant<int, float, std::string, bool>, TokenType> BinaryExpr::evalInt(int x, int y)
{
    switch (op)
    {
        case TokenType::PLUS:
            return std::make_tuple(x + y, TokenType::INT_LIT);
        case TokenType::MINUS:
            return std::make_tuple(x - y, TokenType::INT_LIT);
        case TokenType::STAR:
            return std::make_tuple(x * y, TokenType::INT_LIT);
        case TokenType::SLASH:
            return std::make_tuple(x / y, TokenType::INT_LIT);
    }
}

std::tuple<std::variant<int, float, std::string, bool>, TokenType> BinaryExpr::evalFloat(float x, float y)
{
    switch (op)
    {
        case TokenType::PLUS:
            return std::make_tuple(x + y, TokenType::FLOAT_LIT);
        case TokenType::MINUS:
            return std::make_tuple(x - y, TokenType::FLOAT_LIT);
        case TokenType::STAR:
            return std::make_tuple(x * y, TokenType::FLOAT_LIT);
        case TokenType::SLASH:
            return std::make_tuple(x / y, TokenType::FLOAT_LIT);
    }
}

std::tuple<std::variant<int, float, std::string, bool>, TokenType> BinaryExpr::evalString(std::string x, std::string y)
{
    switch (op)
    {
        case TokenType::PLUS:
            return std::make_tuple(x + y, TokenType::STR_LIT);
        case TokenType::MINUS:
            throw "Cannot do '-' operation on type of STRING";
        case TokenType::STAR:
            throw "Cannot do '*' operation on type of STRING";
        case TokenType::SLASH:
            throw "Cannot do '/' operation on type of STRING";
    }
}


std::tuple<std::variant<int, float, std::string, bool>, TokenType> UnaryExpr::evaluate() 
{
}

std::tuple<std::variant<int, float, std::string, bool>, TokenType>Grouping::evaluate() 
{
    return expr->evaluate();
}

std::tuple<std::variant<int, float, std::string, bool>, TokenType> IntExpr::evaluate() 
{
    return std::make_tuple(val, TokenType::INT_LIT);
}

std::tuple<std::variant<int, float, std::string, bool>, TokenType> FloatExpr::evaluate() 
{
    return std::make_tuple(val, TokenType::FLOAT_LIT);
}

std::tuple<std::variant<int, float, std::string, bool>, TokenType> StringExpr::evaluate() 
{
    return std::make_tuple(val, TokenType::STR_LIT);
}

std::tuple<std::variant<int, float, std::string, bool>, TokenType> BoolExpr::evaluate() 
{
    return std::make_tuple(val, TokenType::BOOL);
}
