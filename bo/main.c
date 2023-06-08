#include <stdio.h>

#include "./Common/common.h"
#include "./Chunk/chunk.h"
#include "./Debug/debug.h"
#include "./VM/vm.h"

int main(int argc, const char* argv[])
{
    initVM();
    Chunk chunk;
    initChunk(&chunk);
    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    writeChunk(&chunk, OP_RETURN, 123);
    disassembleChunk(&chunk, "Test Chunk");
    freeVM();
    interpret(&chunk);
    freeChunk(&chunk);
    return 0;
}
