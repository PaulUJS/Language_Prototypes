#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "./Lexer/lexer.hpp"
#include "./Parser/parser.hpp"

std::string readFile()
{ 
    char ch;
    std::string file;
    std::fstream fin("test.txt", std::fstream::in);
    while (fin >> std::noskipws >> ch)
    {
        file.append(1, ch);
    }
    return file;
}

int main()
{
    auto file = readFile();
    std::cout << file;
    Lexer lexer(file);
    auto tokens = lexer.scanTokens();
    Parser parser(tokens, lexer.debug);
    auto [ast, debug] = parser.parse();
    std:: cout << ast.size() << "\n"; 
    for (unsigned int i = 0; i < ast.size(); i++)
    {
        if (ast[i] != nullptr)
        {
            std::cout << "CHECKING INDEX " << i << "\n";
            try 
            {
                auto [left, right] = ast[i]->evaluate();
                switch (right)
                {
                    case TokenType::INT_LIT:
                        std::cout << std::get<int>(left) << "\n";
                        break;
                    case TokenType::FLOAT_LIT:
                        std::cout << std::get<float>(left) << "\n";
                        break;
                    case TokenType::STR_LIT:
                        std::cout << std::get<std::string>(left) << "\n";
                    case TokenType::BOOL:
                        std::cout << std::get<bool>(left) << "\n";
                }
            }
            catch (std::string msg)
            {
                std::cout << msg << "\n";
                return 1;
            }
        }
        else 
        {
            std::cout << "NULL IN MAIN\n";
        }
    }
    
    return 0;
}
