#ifndef CHUNK_H
#define CHUNK_H

#include <stdlib.h>
#include <GLFW/glfw3.h>

#define CHUNK_SIZE 32
#define CHUNK_TOTAL_BLOCK_COUNT (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_VERTEX_COUNT (CHUNK_TOTAL_BLOCK_COUNT * (3 * 8) * sizeof(float))

typedef struct {
    uint16_t* blocks;
    int index_count;
} chunk_t;

void init_chunk(chunk_t* chunk);

float* get_chunk_vertices(int* count);
GLuint* get_chunk_block_indices(chunk_t* chunk);

#endif