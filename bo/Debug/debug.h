#ifndef DEBUG_H
#define DEBUG_H

#include "../Chunk/chunk.h"

void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);

#endif
