#pragma once

#include "../Lexer/lexer.hpp"

#include <iostream>
#include <memory>

class Expression 
{
    public:
        virtual ~Expression() = default;
        virtual void Print() = 0;
        virtual std::variant<int, float, std::string, bool> Evaluate() = 0;
        virtual TokenType Get_Type() = 0;
};

class BinaryExpr : public Expression 
{
    private:
        std::unique_ptr<Expression> left, right;
        TokenType op;
        TokenType objtype;

        int Eval_Int();
        float Eval_Float();
        std::string Eval_String();
        bool Eval_Bool();

    public:
        BinaryExpr(std::unique_ptr<Expression> left, TokenType& op, std::unique_ptr<Expression> right)
            : left(std::move(left)), op(op), right(std::move(right)) { Evaluate(); }
        ~BinaryExpr() {}

        void Print() override { left->Print(); right->Print(); }
        std::variant<int, float, std::string, bool> Evaluate() override;
        TokenType Get_Type() override { return objtype; }
};

class UnaryExpr : public Expression 
{
    private:
        TokenType op;
        std::unique_ptr<Expression> right;

    public:
        UnaryExpr(TokenType op, std::unique_ptr<Expression> right)
            : op(op), right(std::move(right)) {}
        ~UnaryExpr() {}

        void Print() override { right->Print(); }
        std::variant<int, float, std::string, bool> Evaluate() override {}
        TokenType Get_Type() override { return INT_LIT; }
};

class IntExpr : public Expression 
{
    private:
        int val;

    public:
        IntExpr(int val)
            : val(val) { }
        ~IntExpr() {}

        void Print() override { std::cout << "INT VALUE " << val << "\n"; }
        std::variant<int, float, std::string, bool> Evaluate() override; 
        TokenType Get_Type() override { return INT; }
};

class FloatExpr : public Expression
{ 
    private:
        float val;

    public:
        FloatExpr(float val)
            : val(val) { }
        ~FloatExpr() {}

        void Print() override { std::cout << "FLOAT VALUE " << val << "\n"; }
        std::variant<int, float, std::string, bool> Evaluate() override;
        TokenType Get_Type() override { return FLOAT; }
};

class StringExpr : public Expression 
{
    private:
        std::string val;

    public:
        StringExpr(std::string val)
            : val(val) { }
        ~StringExpr() {}

        void Print() override { std::cout << "STRING VALUE " << val << "\n"; }
        std::variant<int, float, std::string, bool> Evaluate() override;
        TokenType Get_Type() override { return STRING; }
};

class BoolExpr : public Expression 
{
    private:
        bool val;

    public:
        BoolExpr(bool val)
            : val(val) {}
        ~BoolExpr() {}

        void Print() override { std::cout << "BOOL VALUE " << val << "\n"; }
        std::variant<int, float, std::string, bool> Evaluate() override;
        TokenType Get_Type() override { return BOOL; }
};

class NullExpr : public Expression 
{
    private:
        bool val;

    public:
        NullExpr()
            : val(false) {}
        ~NullExpr() {}

        void Print() override { std::cout << "NULLVALUE \n"; }
        std::variant<int, float, std::string, bool> Evaluate() override {}
        TokenType Get_Type() override { return EOF_TOK; }
};
