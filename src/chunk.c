#include "chunk.h"

void init_chunk(chunk_t* chunk) {
    chunk = (chunk_t*)malloc(sizeof(chunk_t));
    chunk->blocks = (uint16_t*)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * sizeof(uint16_t));

    for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) {
        chunk->blocks = 0;
    }
}

float* get_chunk_vertices(int* count) {
    // block count * (values per vertex * vertex count) * sizeof(float)
    float* verts = (float*)malloc(CHUNK_TOTAL_BLOCK_COUNT * (3 * 8) * sizeof(float));

    int verts_index = 0;
    float x_offset, y_offset, z_offset;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                x_offset = x + 1.0f;
                y_offset = y + 1.0f;
                z_offset = z + 1.0f;

                /*
                0, 0, 1
                1, 0, 1
                1, 1, 1
                0, 1, 1

                0, 0, 0
                1, 0, 0
                1, 1, 0
                0, 1, 0
                */
                float single_block_verts[] = {
                    x_offset, y_offset, z_offset + 1,
                    x_offset + 1, y_offset, z_offset + 1,
                    x_offset + 1, y_offset + 1, z_offset +1,
                    x_offset, y_offset + 1, z_offset + 1,

                    x_offset, y_offset, z_offset,
                    x_offset + 1, y_offset, z_offset,
                    x_offset + 1, y_offset + 1, z_offset,
                    x_offset, y_offset + 1, z_offset
                };

                for (int i = 0; i < 3 * 8; i++) {
                    verts[verts_index++] = single_block_verts[i];
                }
            }
        }
    }

    *count = verts_index + 1;
    return verts;
}

GLuint* get_chunk_block_indices(chunk_t* chunk) {
    // TODO: Could probably save some memory with realloc, chunk will never have this many faces visible
    GLuint* indices = (GLuint*)malloc(CHUNK_TOTAL_BLOCK_COUNT * sizeof(GLuint) * 6 * 6);
    int index = 0;

    /*
        0, 1, 2,
        2, 3, 0,
        
        1, 5, 6,
        6, 2, 1,
        
        7, 6, 5,
        5, 4, 7,
        
        4, 0, 3,
        3, 7, 4,
        
        4, 5, 1,
        1, 0, 4,
        
        3, 2, 6,
        6, 7, 3
    */
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                GLuint single_block_indices[] = {
                    0, 1, 2,
                    2, 3, 0,

                    1, 5, 6,
                    6, 2, 1,

                    7, 6, 5,
                    5, 4, 7,

                    4, 0, 3,
                    3, 7, 4,

                    4, 5, 1,
                    1, 0, 4,

                    3, 2, 6,
                    6, 7, 3
                };

                for (int i = 0; i < 6 * 6; i++) {
                    indices[index++] = single_block_indices[i] + 8 * (x + CHUNK_SIZE * (y + CHUNK_SIZE * z));
                }
            }
        }
    }

    chunk->index_count = index + 1;
    return indices;
}
