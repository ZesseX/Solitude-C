#ifndef RENDER_CHUNK_T
#define RENDER_CHUNK_T

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../world/world.h"
#include "../world/chunk.h"

typedef struct {
    GLuint shader_id;
    GLuint texture_id; // TODO: Add array of texture_ids for block animations

    GLuint texture_loc;
    GLuint mvp_matrix_loc;
} render_chunk_context_t;

typedef struct {
    GLuint vbo;
    GLuint vao;
    GLuint ebo;

    int indices;

    chunk_t* chunk;
} render_chunk_t;

int update_render_chunk(chunk_t* chunk, render_chunk_t* rc);

void get_block_visible_faces(chunk_t* chunk, vec3_t block, int* count);

void push_block_vertices(int face, vec3_t pos, GLfloat* verts, int* count);
void push_block_indices(int offset, int face, GLuint* indices, int* count);

void free_render_chunk_buffers(render_chunk_t* rc);

#endif