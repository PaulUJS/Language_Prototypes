#include "chunk.hpp"

#include <iostream>

void Chunk::writeChunk(uint8_t byte)
{
    code.push_back(byte);
}

void Chunk::disassembleChunk(const string& name)
{
    cout << "== " << name << " ==\n";
    for (int i = 0; i < code.size();)
    {
        i = disassembleInstruction(i);
    }
}

static int simpleInstruction(const string& name, int offset)
{
    cout << name << "\n";
    return offset + 1;
}

int Chunk::disassembleInstruction(int offset)
{
    cout << offset << "\n";

    uint8_t instruction = code[offset];
    switch (instruction)
    {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            cout << "Unknown opcode " << instruction << "\n";
            return offset + 1;
    }
}
