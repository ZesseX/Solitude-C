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

#include <time.h>

GLfloat* get_chunk_vertices(int* count) {
    clock_t begin = clock();

    // block count * (values per vertex * vertex count) * sizeof(float)
    GLfloat* verts = (GLfloat*)malloc(CHUNK_TOTAL_BLOCK_COUNT * (3 * 8) * sizeof(GLfloat));

    int verts_index = 0;
    GLfloat x_offset, y_offset, z_offset;
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
                GLfloat single_block_verts[] = {
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

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time: %f", time_spent);

    *count = verts_index;
    return verts;
}

GLuint* get_chunk_block_indices(chunk_t* chunk) {
    // TODO: Could probably save some memory with realloc, chunk will never have this many faces visible
    // Only allocate x amount and if it's exceeded add y amount of extra memory there
    // TODO: GLshort should be enough? at least with 32x32x32
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

    int print_val_count = 5;

    vec3_t block;
    int block_tri_count = 0;
    GLuint* block_visible_tris = (GLuint*)malloc(6 * 6 * sizeof(GLuint));
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                /*
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
                */
                if (chunk->blocks[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] == 0) {
                    continue;
                }
                
                block.x = x;
                block.y = y;
                block.z = z;
                
                get_block_visible_tris(chunk, block, block_visible_tris, &block_tri_count);

                int index_start = 8 * (x + CHUNK_SIZE * (y + CHUNK_SIZE * z));
                for (int i = 0; i < block_tri_count; i++) {
                    indices[index++] = index_start + block_visible_tris[i];
                }
            }
        }
    }

    free(block_visible_tris);

    GLuint* final_indices = (GLuint*)realloc(indices, index * sizeof(GLuint));
    if (final_indices == NULL) {
        fprintf(stderr, "Unable to reallocate memory for chunk indices\n");
        return NULL;
    }

    chunk->index_count = index + 1;
    return final_indices;
}

void get_block_visible_tris(chunk_t* chunk, vec3_t block, GLuint* tris, int* count) {
    *count = 0;

    // TODO: For some reason I need to add x+ face to the z+, x- to the z- and the otherway around,
    // shouldnt be working like that, the order of push_tris_into_arrays should 0 to 5 not 1,0,3,2,4,5
    if(!has_block(chunk, (vec3_t){block.x, block.y, block.z + 1})){
        push_tris_into_array(1, tris, count);
    }
    if(!has_block(chunk, (vec3_t){block.x + 1, block.y, block.z})){
        push_tris_into_array(0, tris, count);
    }
    if(!has_block(chunk, (vec3_t){block.x, block.y, block.z - 1})){
        push_tris_into_array(3, tris, count);
    }
    if(!has_block(chunk, (vec3_t){block.x - 1, block.y, block.z})){
        push_tris_into_array(2, tris, count);
    }
    if(!has_block(chunk, (vec3_t){block.x, block.y - 1, block.z})){
        push_tris_into_array(4, tris, count);
    }
    if(!has_block(chunk, (vec3_t){block.x, block.y + 1, block.z})){
        push_tris_into_array(5, tris, count);
    }
}

static int has_block(chunk_t* chunk, vec3_t pos) {
    if(pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x == CHUNK_SIZE || pos.y == CHUNK_SIZE || pos.z == CHUNK_SIZE) {
        // TODO: Check the nearby chunk
        return 0;
    }

    int index = pos.x + CHUNK_SIZE * (pos.y + CHUNK_SIZE * pos.z);
    return chunk->blocks[index] != 0;
}

static void push_tris_into_array(int face, GLuint* arr, int* index){
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
    switch(face){
        case 0:
            arr[(*index)++] = 0;
            arr[(*index)++] = 1;
            arr[(*index)++] = 2;

            arr[(*index)++] = 2;
            arr[(*index)++] = 3;
            arr[(*index)++] = 0;
            break;
        case 1:
            arr[(*index)++] = 1;
            arr[(*index)++] = 5;
            arr[(*index)++] = 6;

            arr[(*index)++] = 6;
            arr[(*index)++] = 2;
            arr[(*index)++] = 1;
            break;
        case 2:
            arr[(*index)++] = 7;
            arr[(*index)++] = 6;
            arr[(*index)++] = 5;

            arr[(*index)++] = 5;
            arr[(*index)++] = 4;
            arr[(*index)++] = 7;
            break;
        case 3:
            arr[(*index)++] = 4;
            arr[(*index)++] = 0;
            arr[(*index)++] = 3;

            arr[(*index)++] = 3;
            arr[(*index)++] = 7;
            arr[(*index)++] = 4;
            break;
        case 4:
            arr[(*index)++] = 4;
            arr[(*index)++] = 5;
            arr[(*index)++] = 1;

            arr[(*index)++] = 1;
            arr[(*index)++] = 0;
            arr[(*index)++] = 4;
            break;
        case 5:
            arr[(*index)++] = 3;
            arr[(*index)++] = 2;
            arr[(*index)++] = 6;

            arr[(*index)++] = 6;
            arr[(*index)++] = 7;
            arr[(*index)++] = 3;
            break;
    }
}
