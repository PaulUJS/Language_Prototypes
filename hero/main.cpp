#include <iostream>

#include "./Lexer/lexer.hpp"
#include "./Parser/parser.hpp"

int main()
{
    std::cout << "hello world \n";
    Lexer lexer;
    auto tokens = lexer.Tokenize();
    
    Parser parser(tokens);
    auto expr = parser.Parse();
    for (unsigned int i = 0; i < expr.size(); i++)
    {
        expr[i]->Print();
    }
    return 0;
}
