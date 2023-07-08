#include <iostream>

#include "./Lexer/lexer.hpp"
#include "./Chunk/chunk.hpp"

int main()
{
    Chunk chunk;
    chunk.writeChunk(OP_RETURN);
    chunk.disassembleChunk("TEST CHUNK");
    return 0;
}
