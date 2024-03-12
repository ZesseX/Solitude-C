#ifndef CHUNK_H
#define CHUNK_H

#include "world_defines.h"
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

void init_chunk(chunk_t* chunk, vec3_t pos);

int has_block(chunk_t* chunk, vec3_t pos);

int chunk_vec3_to_index(vec3_t pos);
vec3_t chunk_index_to_vec3(int index);

#endif