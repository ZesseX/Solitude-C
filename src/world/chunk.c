#include "chunk.h"

void init_chunk(chunk_t* chunk) {
    //chunk = (chunk_t*)malloc(sizeof(chunk_t));
    chunk->blocks = (uint16_t*)malloc(CHUNK_TOTAL_BLOCK_COUNT * sizeof(uint16_t));

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
}

int has_block(chunk_t* chunk, vec3_t pos) {
    if(pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x == CHUNK_SIZE || pos.y == CHUNK_SIZE || pos.z == CHUNK_SIZE) {
        // TODO: Check the nearby chunk
        return 0;
    }

    int index = pos.x + CHUNK_SIZE * (pos.y + CHUNK_SIZE * pos.z);
    return chunk->blocks[index] != 0;
}
