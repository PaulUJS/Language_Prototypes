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
            : left(std::move(left)), op(op), right(std::move(right)) { /* Evaluate(); */ }
        ~BinaryExpr() {}

        void Print() override { left->Print(); right->Print(); }
        std::variant<int, float, std::string, bool> Evaluate() override;
        TokenType Get_Type() override { return objtype; }
};

class UnaryExpr : public Expression 
{
    private:
        TokenType op;
        TokenType objtype;
        std::unique_ptr<Expression> right;

    public:
        UnaryExpr(TokenType op, std::unique_ptr<Expression> right)
            : op(op), right(std::move(right)), objtype(INT) {}
        ~UnaryExpr() {}

        void Print() override { right->Print(); }
        std::variant<int, float, std::string, bool> Evaluate() override {}
        TokenType Get_Type() override { return objtype; }
};

class IntExpr : public Expression 
{
    private:
        int val;
        TokenType objtype;

    public:
        IntExpr(int val)
            : val(val), objtype(INT) { }
        ~IntExpr() {}

        void Print() override { std::cout << "INT VALUE " << val << "\n"; }
        std::variant<int, float, std::string, bool> Evaluate() override; 
        TokenType Get_Type() override { return objtype; }
};

class FloatExpr : public Expression
{ 
    private:
        float val;
        TokenType objtype;

    public:
        FloatExpr(float val)
            : val(val), objtype(FLOAT) { }
        ~FloatExpr() {}

        void Print() override { std::cout << "FLOAT VALUE " << val << "\n"; }
        std::variant<int, float, std::string, bool> Evaluate() override;
        TokenType Get_Type() override { return objtype; }
};

class StringExpr : public Expression 
{
    private:
        std::string val;
        TokenType objtype;

    public:
        StringExpr(std::string val)
            : val(val), objtype(STRING) { }
        ~StringExpr() {}

        void Print() override { std::cout << "STRING VALUE " << val << "\n"; }
        std::variant<int, float, std::string, bool> Evaluate() override;
        TokenType Get_Type() override { return objtype; }
};

class BoolExpr : public Expression 
{
    private:
        bool val;
        TokenType objtype;

    public:
        BoolExpr(bool val)
            : val(val), objtype(BOOL) {}
        ~BoolExpr() {}

        void Print() override { std::cout << "BOOL VALUE " << val << "\n"; }
        std::variant<int, float, std::string, bool> Evaluate() override;
        TokenType Get_Type() override { return objtype; }
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

