#include "chunk.h"

void init_chunk(chunk_t* chunk) {
    //chunk = (chunk_t*)malloc(sizeof(chunk_t));
    chunk->blocks = (uint16_t*)malloc(CHUNK_TOTAL_BLOCK_COUNT * sizeof(uint16_t));

    /*
    int even = 1;
    for (int i = 0; i < CHUNK_TOTAL_BLOCK_COUNT; i++) {
        if(even){
            chunk->blocks[i] = 1;
            even = 0;
        }
        else {
            chunk->blocks[i] = 1;
            even = 1;
        }
    }
    */

    /*
    uint16_t block = 0;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            if (y > 16) block = 0;
            else block = 1;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                chunk->blocks[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] = block;
            }
        }
    }
    
    */

   /*
    uint16_t block = 0;
    int x = 0, z = 0;
    for (int y = 0; y < CHUNK_SIZE; y++) {
        float rand_max_height = perlin_noise((vec2_t){x, z});
        int max_height = (int)(CHUNK_SIZE * rand_max_height);
        max_height = 38;
        if (y > max_height) block = 0;
        else block = 1;

        for (; x < CHUNK_SIZE; x++) {
            for (; z < CHUNK_SIZE; z++) {
                chunk->blocks[y * CHUNK_SIZE_SQUARED + x * CHUNK_SIZE + z] = block;
            }
        }
    }
*/

    for (int y = 0; y < CHUNK_SIZE; y++) {
        srand(get_seed() * y);
        float r = (double)rand() / (double)RAND_MAX;

        printf("%.2f", r);

        int height = r * CHUNK_SIZE;
        uint16_t block = height > y ? 0 : 1;

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
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
