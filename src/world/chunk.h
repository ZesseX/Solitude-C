#ifndef CHUNK_H
#define CHUNK_H

#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "../math/vec3.h"
#include "../utils/noise.h"

#define CHUNK_SIZE 38
#define CHUNK_SIZE_SQUARED (CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_TOTAL_BLOCK_COUNT (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)

typedef struct {
    uint16_t* blocks;

    // TODO: Add extra stuff for blocks actively being affected by gravity
} chunk_t;

void init_chunk(chunk_t* chunk, vec2_t pos);

int has_block(chunk_t* chunk, vec3_t pos);

#endif