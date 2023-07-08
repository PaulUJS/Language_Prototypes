#pragma once

#include <vector>
#include <string>

class Debug
{
    public:
        void error(std::string err);
        void outputErrors();
        bool foundError();
    private:
        bool passed;
        std::vector<std::string> errors;
};
