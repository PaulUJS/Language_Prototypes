#include "debug.hpp"

#include <iostream>

void Debug::error(std::string err)
{
    errors.push_back(err);
}

void Debug::outputErrors()
{
    for (unsigned int i = 0; i < errors.size(); i++)
    {
        std::cout << errors[i] << "\n";
    }
}

bool Debug::foundError()
{
    if (!errors.empty())
        return true;
    return false;
}
