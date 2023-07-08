#pragma once

#include "../Lexer/lexer.hpp"

#include <memory>
#include <string>
#include <variant>
#include <iostream>
#include <tuple>

class Expression
{
    public:
        virtual ~Expression() = default;
        virtual std::tuple<std::variant<int, float, std::string, bool>, TokenType> evaluate() = 0;
};

class BinaryExpr : public Expression
{
    public:
        BinaryExpr(std::unique_ptr<Expression> left, TokenType op, std::unique_ptr<Expression> right)
            : left(std::move(left)), op(op), right(std::move(right)) {} 
        std::tuple<std::variant<int, float, std::string, bool>, TokenType> evaluate() override;
    private:
        std::unique_ptr<Expression> left, right;
        TokenType op;

        std::tuple<std::variant<int, float, std::string, bool>, TokenType> evalInt(int x, int y);
        std::tuple<std::variant<int, float, std::string, bool>, TokenType> evalFloat(float x, float y);
        std::tuple<std::variant<int, float, std::string, bool>, TokenType> evalString(std::string x, std::string y);
};

class UnaryExpr : public Expression 
{
    public:
        UnaryExpr(TokenType op, std::unique_ptr<Expression> right)
            : op(op), right(std::move(right)) {}
        std::tuple<std::variant<int, float, std::string, bool>, TokenType> evaluate() override;
    private:
        TokenType op;
        std::unique_ptr<Expression> right;
};

class Grouping : public Expression
{
    public:
        Grouping(std::unique_ptr<Expression> expr)
            : expr(std::move(expr)) {}
        std::tuple<std::variant<int, float, std::string, bool>, TokenType> evaluate() override;
    private:
        std::unique_ptr<Expression> expr;
};

class IntExpr : public Expression 
{
    public:
        IntExpr(int val)
            : val(val)
        {   
        }
        std::tuple<std::variant<int, float, std::string, bool>, TokenType> evaluate() override;
        TokenType getType() { return TokenType::INT_LIT; }
    private:
        int val;
};

class FloatExpr : public Expression 
{
    public:
        FloatExpr(float val)
            : val(val) 
        {   
        }
        std::tuple<std::variant<int, float, std::string, bool>, TokenType> evaluate() override;
        TokenType getType() { return TokenType::FLOAT_LIT; }
    private:
        float val;
};

class StringExpr : public Expression 
{
    public:
        StringExpr(std::string val)
            : val(val)
        {   
        }
        std::tuple<std::variant<int, float, std::string, bool>, TokenType> evaluate() override;
        TokenType getType() { return TokenType::STR_LIT; }
    private:
        std::string val;
};

class BoolExpr : public Expression 
{
    public:
        BoolExpr(bool val)
            : val(val) 
        {   
        }
        std::tuple<std::variant<int, float, std::string, bool>, TokenType> evaluate() override;
        TokenType getType() { return TokenType::BOOL; }
    private:
        bool val;
};

