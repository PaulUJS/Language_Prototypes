#include "lexer.hpp"

#include <fstream>
#include <iostream>

std::vector<Token> Lexer::Tokenize()
{
    char ch;
    bool commenting = false;
    std::string buffer;

    std::fstream fin("test.hero", std::fstream::in);
    while (fin >> std::noskipws >> ch)
    {
        std::cout << ch;
        if (commenting)
        {
            if (ch == '#')
                commenting = false;
        }
        else 
        {
            switch (ch)
            {
                case '+':
                    Add_Token(PLUS);
                    break;
                case '-':
                    Add_Token(MINUS);
                    break;
                case '*':
                    Add_Token(STAR);
                    break;
                case '/':
                    Add_Token(SLASH);
                    break;
                case ';':
                    if (!writing)
                    {
                        if (Match_Identifier(buffer) == false)
                            Match_Token(buffer);
                        Add_Token(SEMICOLON);
                    }
                    break;
                case '\n':
                    if (!writing)
                        if (Match_Identifier(buffer) == false)
                            Match_Token(buffer);
                    line++;
                    break;
                case '#':
                    commenting = true;
                    break;
                case '"':
                    if (!writing)
                        writing = true;
                    else if (writing)
                    {
                        Add_Token(STR_LIT, buffer);
                        writing = false;
                    }
                    break;
                case ' ':
                    if (!writing)
                        if (Match_Identifier(buffer) == false)
                            Match_Token(buffer);
                    else 
                        buffer.append(1, ch);
                    break;
                default:
                    buffer.append(1, ch);
                    break;
            }
        }
    }
    if (Match_Identifier(buffer) == false)
        Match_Token(buffer);
    Add_Token(EOF_TOK);
    fin.close();
    return tokens;
}

bool Lexer::Match_Identifier(std::string& str)
{
    if (!tokens.empty())
    {
        TokenType tok = tokens.back().tokentype;
        if (tok == INT || tok == FLOAT || tok == STRING || tok == BOOL)
        {
            Add_Token(IDENTIFIER, str);
            str.clear();
            return true;
        }
    }
    return false;
}

void Lexer::Match_Token(std::string& str)
{
    if (str.compare("fn") == 0)
        Add_Token(FN, str);
    else if (str.compare("return") == 0)
        Add_Token(RETURN);
    else if (str.compare("int") == 0)
        Add_Token(INT);
    else if (str.compare("float") == 0)
        Add_Token(FLOAT);
    else if (str.compare("string") == 0)
        Add_Token(STRING);
    else if (str.compare("char") == 0)
        Add_Token(CHAR);
    else if (str.compare("bool") == 0)
        Add_Token(BOOL);
    else if (str.compare("true") == 0)
        Add_Token(TRUE);
    else if (str.compare("false") == 0)
        Add_Token(FALSE);
    else if (str.compare("=") == 0)
        Add_Token(EQUAL);
    else if (str.compare("==") == 0)
        Add_Token(EQUAL_EQUAL);
    else if (str.compare("!") == 0)
        Add_Token(BANG);
    else if (str.compare("!=") == 0)
        Add_Token(BANG_EQUAL);
    else if (str.compare(">") == 0)
        Add_Token(GREATER);
    else if (str.compare(">=") == 0)
        Add_Token(GREATER_EQUAL);
    else if (str.compare("<") == 0)
        Add_Token(LESS);
    else if (str.compare("<=") == 0)
        Add_Token(LESS_EQUAL);
    else if (str.compare(" ") == 0)
    {
        str.clear();
        return;
    }
    else 
    {
        if (str.find(".") == std::string::npos)
        {
            try 
            {
                Add_Token(INT_LIT, stoi(str));
                str.clear();
                return;
            }
            catch (...)
            {
            }
        }
        else if (str.find(".") != std::string::npos)
        {
            try 
            {
                Add_Token(FLOAT_LIT, stof(str));
                str.clear();
                return;
            }
            catch (...)
            {
            }
        }
    }
    str.clear();
}


void Lexer::Add_Token(TokenType token)
{
    tokens.push_back({ token, line, 0 });
}

void Lexer::Add_Token(TokenType token, int num)
{
    tokens.push_back({ token, line, num });
}

void Lexer::Add_Token(TokenType token, float num)
{
    tokens.push_back({ token, line, num });
}

void Lexer::Add_Token(TokenType token, std::string str)
{
    tokens.push_back({ token, line, str });
}

void Lexer::Print_Tokens()
{
    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        switch (tokens[i].tokentype)
        {
            case 0:
                std::cout << "EOF \n";
                break;
            case 1:
                std::cout << "FN \n";
                break;
            case 2:
                std::cout << "RETURN \n";
                break;
            case 3:
                std::cout << "INT \n";
                break;
            case 4:
                std::cout << "FLOAT \n";
                break;
            case 5:
                std::cout << "STRING \n";
                break;
            case 6:
                std::cout << "CHAR \n";
                break;
            case 7:
                std::cout << "BOOL \n";
                break;
            case 8:
                std::cout << "TRUE \n";
                break;
            case 9:
                std::cout << "FALSE \n";
                break;
            case 10:
                std::cout << "PLUS \n";
                break;
            case 11:
                std::cout << "MINUS \n";
                break;
            case 12:
                std::cout << "STAR \n";
                break;
            case 13:
                std::cout << "SLASH \n";
                break;
            case 14:
                std::cout << "INT_LIT \n";
                std::cout << std::get<int>(tokens[i].val) << "\n";
                break;
            case 15:
                std::cout << "STRING_LIT \n";
                std::cout << std::get<std::string>(tokens[i].val) << "\n";
                break;
            case 16:
                std::cout << "CHAR_LIT \n";
                break;
            case 17:
                std::cout << "FLOAT_LIT \n";
                std::cout << std::get<float>(tokens[i].val) << "\n";
                break;
            case 18:
                std::cout << "SEMICOLON \n";
                break;
        }
    }
}
