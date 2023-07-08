#include "lexer.hpp"

#include <iostream>

vector<Token> Lexer::tokenize()
{
    char ch;
    bool commenting = false;

    fstream fin("test.txt", fstream::in);
    while (fin >> noskipws >> ch)
    {
        cout << ch;
        if (commenting)
        {
            if (ch == '#')
                commenting = false;
        }
        else
        {
            
        }
    }
    fin.close();
    return tokens;
}
