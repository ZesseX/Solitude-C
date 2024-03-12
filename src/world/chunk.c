#include "chunk.h"

void init_chunk(chunk_t* chunk, vec3_t pos) {
    //chunk = (chunk_t*)malloc(sizeof(chunk_t));
    chunk->blocks = (uint16_t*)malloc(CHUNK_TOTAL_BLOCK_COUNT * sizeof(uint16_t));

    int max_heights[CHUNK_SIZE][CHUNK_SIZE];
    int full_air = 1;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            // TODO: Include world heightmap in this
            max_heights[x][z] = CHUNK_SIZE * perlin_noise(x + (pos.x * CHUNK_SIZE), z + (pos.y * CHUNK_SIZE), 0.2f, 2);
            // TODO: Fix this max height thing
            if (max_heights[x][z] != 0) full_air = 0;
        }
    }

    // TODO: Just add a boolean for this in the chunk data and check that in the rendering code
    if (full_air) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    chunk->blocks[chunk_vec3_to_index((vec3_t){x, y, z})] = 0;
                }
            }
        }
    }
    else {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    uint16_t block = (y > max_heights[x][z]) ? 0 : 1;
                    chunk->blocks[chunk_vec3_to_index((vec3_t){x, y, z})] = block;
                }
            }
        }
    }
}

int has_block(chunk_t* chunk, vec3_t pos) {
    if(pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x == CHUNK_SIZE || pos.y == CHUNK_SIZE || pos.z == CHUNK_SIZE) {
        // TODO: Check the nearby chunk
        return 0;
    }

    return chunk->blocks[chunk_vec3_to_index(pos)] != 0;
}

int chunk_vec3_to_index(vec3_t pos) {
    return pos.y * CHUNK_SIZE_SQUARED + pos.x * CHUNK_SIZE + pos.z;
}

vec3_t chunk_index_to_vec3(int index) {
    vec3_t pos;
    pos.y = index / (CHUNK_SIZE_SQUARED);
    pos.x = (index % (CHUNK_SIZE_SQUARED)) / CHUNK_SIZE;
    pos.z = (index % (CHUNK_SIZE_SQUARED)) % CHUNK_SIZE;
    return pos;
}
