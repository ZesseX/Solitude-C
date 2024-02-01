#ifndef CHUNK_H
#define CHUNK_H

#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "../math/vec3.h"

#define CHUNK_SIZE 32
#define CHUNK_TOTAL_BLOCK_COUNT (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_VERTEX_COUNT (CHUNK_TOTAL_BLOCK_COUNT * 24 * sizeof(GLuint))

typedef struct {
    uint16_t* blocks;
} chunk_t;

void init_chunk(chunk_t* chunk);

int has_block(chunk_t* chunk, vec3_t pos);

#endif