#include "chunk.h"

void init_chunk(chunk_t* chunk, vec3_t pos) {
    chunk->position = pos;
    chunk->position_real = vec3_mulf(pos, CHUNK_SIZE);
    chunk->blocks = (uint16_t*)malloc(CHUNK_TOTAL_BLOCK_COUNT * sizeof(uint16_t));

    int full_air = 1;
    int max_heights[CHUNK_SIZE][CHUNK_SIZE];
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            // TODO: Include world heightmap in this
            int height = WORLD_MAX_PERLIN_HEIGHT * perlin_noise(x + chunk->position_real.x, z + chunk->position_real.z, 0.2f, 2);
            max_heights[x][z] = height - chunk->position_real.y;

            if (full_air && height - chunk->position_real.y < 0) full_air = 0;
        }
    }

    if (full_air) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    chunk->blocks[chunk_vec3_to_index((vec3_t){x, y, z})] = 1;
                }
            }
        }
    }

    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                uint16_t block = (y > max_heights[x][z]) ? 0 : 1;
                chunk->blocks[chunk_vec3_to_index((vec3_t){x, y, z})] = block;
            }
        }
    }
}

int has_block(chunk_t* chunk, vec3_t pos) {
    if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x == CHUNK_SIZE || pos.y == CHUNK_SIZE || pos.z == CHUNK_SIZE) {
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

void delete_chunk(chunk_t* chunk) {
    free(chunk->blocks);
    free(chunk);
}
