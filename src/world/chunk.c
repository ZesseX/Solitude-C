#include "chunk.h"

void init_chunk(chunk_t* chunk, vec2_t pos) {
    //chunk = (chunk_t*)malloc(sizeof(chunk_t));
    chunk->blocks = (uint16_t*)malloc(CHUNK_TOTAL_BLOCK_COUNT * sizeof(uint16_t));

    int max_heights[CHUNK_SIZE][CHUNK_SIZE];

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            max_heights[x][z] = CHUNK_SIZE * perlin_noise(x + (pos.x * CHUNK_SIZE), z + (pos.y * CHUNK_SIZE), 0.2f, 2);
        }
    }

    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                uint16_t block = (y > max_heights[x][z]) ? 0 : 1;
                chunk->blocks[y * CHUNK_SIZE_SQUARED + x * CHUNK_SIZE + z] = block;
            }
        }
    }
}

int has_block(chunk_t* chunk, vec3_t pos) {
    if(pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x == CHUNK_SIZE || pos.y == CHUNK_SIZE || pos.z == CHUNK_SIZE) {
        // TODO: Check the nearby chunk
        return 0;
    }

    int index = pos.y * CHUNK_SIZE_SQUARED + pos.x * CHUNK_SIZE + pos.z;
    return chunk->blocks[index] != 0;
}
