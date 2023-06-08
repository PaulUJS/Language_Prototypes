#include "ast.hpp"

std::variant<int, float, std::string, bool> BinaryExpr::Evaluate()
{
    TokenType ltok = left->Get_Type();
    TokenType rtok = right->Get_Type();
    switch (rtok)
    {
        case INT:
            return Eval_Int();
        case FLOAT:
            return Eval_Float();
        case STRING:
            return Eval_String();
        case BOOL:
            return Eval_Bool();
    }
    std::cout << "LEFT " << ltok << "RIGHT " << rtok << "\n";
}

int BinaryExpr::Eval_Int()
{   
    int res = 0;
    objtype = INT;
    switch (op)
    {
        case PLUS:
            res = std::get<int>(left->Evaluate()) + std::get<int>(right->Evaluate());
            std::cout << res << "\n";
            return res;
        case MINUS:
            res = std::get<int>(left->Evaluate()) - std::get<int>(right->Evaluate());
            std::cout << res << "\n";
            return res;
        case STAR:
            res = std::get<int>(left->Evaluate()) * std::get<int>(right->Evaluate());
            std::cout << res << "\n";
            return res;
        case SLASH:
            res = std::get<int>(left->Evaluate()) / std::get<int>(right->Evaluate());
            std::cout << res << "\n";
            return res;
    }
}

float BinaryExpr::Eval_Float()
{
    float res = 0.0f;
    objtype = FLOAT;
    switch (op)
    {
        case PLUS:
            res = std::get<float>(left->Evaluate()) + std::get<float>(right->Evaluate());
            std::cout << res << "\n";
            return res;
        case MINUS:
            res = std::get<float>(left->Evaluate()) - std::get<float>(right->Evaluate());
            std::cout << res << "\n";
            return res;
        case STAR:
            res = std::get<float>(left->Evaluate()) * std::get<float>(right->Evaluate());
            std::cout << res << "\n";
            return res;
        case SLASH:
            res = std::get<float>(left->Evaluate()) / std::get<float>(right->Evaluate());
            std::cout << res << "\n";
            return res;
    }
}

std::string BinaryExpr::Eval_String()
{

}

bool BinaryExpr::Eval_Bool()
{
    
}


std::variant<int, float, std::string, bool> IntExpr::Evaluate()
{
    return this->val;
}

std::variant<int, float, std::string, bool> FloatExpr::Evaluate()
{
    return this->val;
}

std::variant<int, float, std::string, bool> StringExpr::Evaluate()
{
    std::cout << "String Val " << this->val << "\n";
    return this->val;
}

std::variant<int, float, std::string, bool> BoolExpr::Evaluate()
{
    std::cout << "Bool Val " << this-> val << "\n";
    return this->val;
}

