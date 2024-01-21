#ifndef CHUNK_H
#define CHUNK_H

#include <stdlib.h>
#include <GLFW/glfw3.h>

#define CHUNK_SIZE 32

typedef struct {
    uint16_t* blocks;
} chunk_t;

void init_chunk(chunk_t* chunk);

#endif