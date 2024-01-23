#ifndef CHUNK_H
#define CHUNK_H

#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "math/vec3.h"

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

void get_block_visible_tris(chunk_t* chunk, vec3_t block, GLuint* GLuint, int* count);

static int has_block(chunk_t* chunk, vec3_t pos);
static void push_tris_into_array(int face, GLuint* array, int* index);

#endif