#ifndef RENDER_CHUNK_T
#define RENDER_CHUNK_T

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../world/chunk.h"

typedef struct {
    GLuint shader_id;
    GLuint texture_id;

    GLuint texture_loc;
    GLuint mvp_matrix_loc;
} chunk_render_context_t;

typedef struct {
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
} render_chunk_t;

int update_render_chunk(chunk_t* chunk, render_chunk_t* rc);

int* get_block_visible_faces(chunk_t* chunk, vec3_t block, int* count);

#endif