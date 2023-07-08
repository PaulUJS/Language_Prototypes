#pragma once

#include <vector>
#include <string>

using namespace std;

enum OpCode
{
    OP_RETURN,
};

class Chunk
{
    public:
        vector<uint8_t> code;

        void writeChunk(uint8_t byte);
        void disassembleChunk(const string& name);
        int disassembleInstruction(int offset);
};
