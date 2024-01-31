#include "render_chunk.h"

int update_render_chunk(chunk_t* chunk, render_chunk_t* rc) {
    GLfloat* verts = (GLfloat*)malloc(CHUNK_TOTAL_BLOCK_COUNT * (3 * 8) * sizeof(GLfloat));
    int verts_count = 0;

    GLuint* indices = (GLint*)malloc(CHUNK_TOTAL_BLOCK_COUNT * (6 * 6) * sizeof(GLuint));
    int indices_count = 0;

    int visible_faces[6];
    vec3_t pos;
    for (int i = 0; i < CHUNK_TOTAL_BLOCK_COUNT; i++) {
        pos.x = i % CHUNK_SIZE;
        pos.y = (i / CHUNK_SIZE) % CHUNK_SIZE;
        pos.z = i / (CHUNK_SIZE * CHUNK_SIZE);

        get_block_visible_faces(chunk, pos, &visible_faces);
    }
}

void get_block_visible_faces(chunk_t* chunk, vec3_t block, int* faces) {
    faces[1] = !has_block(chunk, (vec3_t){block.x, block.y, block.z + 1}) ? 1 : 0;
    faces[0] = !has_block(chunk, (vec3_t){block.x + 1, block.y, block.z}) ? 1 : 0;
    faces[3] = !has_block(chunk, (vec3_t){block.x, block.y, block.z - 1}) ? 1 : 0;
    faces[2] = !has_block(chunk, (vec3_t){block.x - 1, block.y, block.z}) ? 1 : 0;
    faces[4] = !has_block(chunk, (vec3_t){block.x, block.y - 1, block.z}) ? 1 : 0;
    faces[5] = !has_block(chunk, (vec3_t){block.x, block.y + 1, block.z}) ? 1 : 0;
}