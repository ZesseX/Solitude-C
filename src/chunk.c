#include "chunk.h"

void init_chunk(chunk_t* chunk) {
    chunk = (chunk_t*)malloc(sizeof(chunk_t));
    chunk->blocks = (uint16_t*)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * sizeof(uint16_t));

    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) {
        chunk->blocks = 0;
    }
}