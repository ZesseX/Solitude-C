#include "render_chunk.h"

int update_render_chunk_init(chunk_t* chunk, render_chunk_t* rc, GLfloat** verts, GLuint** indices, int* num_verts, int* num_indices) {
    rc->chunk = chunk;

    (*verts) = (GLfloat*)malloc(CHUNK_TOTAL_BLOCK_COUNT * ((3 + 2) * 4 * 6) * sizeof(GLfloat));
    *num_verts = 0;

    *indices = (GLuint*)malloc(CHUNK_TOTAL_BLOCK_COUNT * (6 * 6) * sizeof(GLuint));
    *num_indices = 0;

    int visible_faces[6];
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if(rc->chunk->blocks[y * CHUNK_SIZE_SQUARED + x * CHUNK_SIZE + z] == 0) {
                    continue;
                }

                get_block_visible_faces(rc->chunk, (vec3_t){x, y, z}, visible_faces);
                for (int i = 0; i < 6; i++){
                    if(!visible_faces[i]) continue;

                    push_block_vertices(i, (vec3_t){x, y, z}, *verts, num_verts);
                    push_block_indices(((*num_verts) / (3 + 2)) - 4, i, *indices, num_indices);
                }
            }
        }  
    }

    // TODO: Check for failure
    if (*num_verts != 0) {
        (*verts) = (GLfloat*)realloc((*verts), sizeof(GLfloat) * (*num_verts));
        (*indices) = (GLuint*)realloc((*indices), sizeof(GLuint) * (*num_indices));
    }

    return 1;
}

int update_render_chunk_opengl(chunk_t* chunk, render_chunk_t* rc, GLfloat** verts, GLuint** indices, int num_verts, int num_indices) {
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * num_verts, (*verts), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * num_indices, (*indices), GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
   
    rc->vao = vao;
    rc->vbo = vbo;
    rc->ebo = ebo;

    rc->indices = num_indices;

    // TODO: Why the fuck freeing this thing causes segmentation fault?!
    //free(*verts);
    //free(*indices);

    return 1;
}

void get_block_visible_faces(chunk_t* chunk, vec3_t block, int* faces) {
    faces[0] = !has_block(chunk, (vec3_t){block.x, block.y, block.z + 1}) ? 1 : 0;
    faces[1] = !has_block(chunk, (vec3_t){block.x + 1, block.y, block.z}) ? 1 : 0;
    faces[2] = !has_block(chunk, (vec3_t){block.x, block.y, block.z - 1}) ? 1 : 0;
    faces[3] = !has_block(chunk, (vec3_t){block.x - 1, block.y, block.z}) ? 1 : 0;
    faces[4] = !has_block(chunk, (vec3_t){block.x, block.y - 1, block.z}) ? 1 : 0;
    faces[5] = !has_block(chunk, (vec3_t){block.x, block.y + 1, block.z}) ? 1 : 0;
}

void push_block_vertices(int face, vec3_t pos, GLfloat* verts, int* count) {
    /* vertex offsets
        0, 0, 1
        1, 0, 1
        1, 1, 1
        0, 1, 1

        0, 0, 0
        1, 0, 0
        1, 1, 0
        0, 1, 0
    */

    #define VERTEX_0_OFFSET (vec3_t){0.0f, 0.0f, 1.0f}
    #define VERTEX_1_OFFSET (vec3_t){1.0f, 0.0f, 1.0f}
    #define VERTEX_2_OFFSET (vec3_t){1.0f, 1.0f, 1.0f}
    #define VERTEX_3_OFFSET (vec3_t){0.0f, 1.0f, 1.0f}

    #define VERTEX_4_OFFSET (vec3_t){0.0f, 0.0f, 0.0f}
    #define VERTEX_5_OFFSET (vec3_t){1.0f, 0.0f, 0.0f}
    #define VERTEX_6_OFFSET (vec3_t){1.0f, 1.0f, 0.0f}
    #define VERTEX_7_OFFSET (vec3_t){0.0f, 1.0f, 0.0f}

    vec3_t vert_pos[4];
    switch(face) {
        case 0:
            vert_pos[0] = vec3_add(pos, VERTEX_0_OFFSET);
            vert_pos[1] = vec3_add(pos, VERTEX_1_OFFSET);
            vert_pos[2] = vec3_add(pos, VERTEX_2_OFFSET);
            vert_pos[3] = vec3_add(pos, VERTEX_3_OFFSET);
            break;
        case 1:
            vert_pos[0] = vec3_add(pos, VERTEX_1_OFFSET);
            vert_pos[1] = vec3_add(pos, VERTEX_2_OFFSET);
            vert_pos[2] = vec3_add(pos, VERTEX_5_OFFSET);
            vert_pos[3] = vec3_add(pos, VERTEX_6_OFFSET);
            break;
        case 2:
            vert_pos[0] = vec3_add(pos, VERTEX_4_OFFSET);
            vert_pos[1] = vec3_add(pos, VERTEX_5_OFFSET);
            vert_pos[2] = vec3_add(pos, VERTEX_6_OFFSET);
            vert_pos[3] = vec3_add(pos, VERTEX_7_OFFSET);
            break;
        case 3:
            vert_pos[0] = vec3_add(pos, VERTEX_0_OFFSET);
            vert_pos[1] = vec3_add(pos, VERTEX_3_OFFSET);
            vert_pos[2] = vec3_add(pos, VERTEX_4_OFFSET);
            vert_pos[3] = vec3_add(pos, VERTEX_7_OFFSET);
            break;
        case 4:
            vert_pos[0] = vec3_add(pos, VERTEX_0_OFFSET);
            vert_pos[1] = vec3_add(pos, VERTEX_1_OFFSET);
            vert_pos[2] = vec3_add(pos, VERTEX_4_OFFSET);
            vert_pos[3] = vec3_add(pos, VERTEX_5_OFFSET);
            break;
        case 5:
            vert_pos[0] = vec3_add(pos, VERTEX_2_OFFSET);
            vert_pos[1] = vec3_add(pos, VERTEX_3_OFFSET);
            vert_pos[2] = vec3_add(pos, VERTEX_6_OFFSET);
            vert_pos[3] = vec3_add(pos, VERTEX_7_OFFSET);
            break;
    }

    #define VERTEX_UV_BOTTOM_LEFT       (vec2_t) {0.0f, 0.0f}
    #define VERTEX_UV_BOTTOM_RIGHT      (vec2_t) {1.0f, 0.0f}
    #define VERTEX_UV_TOP_LEFT          (vec2_t) {0.0f, 1.0f}
    #define VERTEX_UV_TOP_RIGHT         (vec2_t) {1.0f, 1.0f}

    vec2_t vert_uv[4];
    switch (face) {
        case 0:
            vert_uv[0] = VERTEX_UV_BOTTOM_LEFT;
            vert_uv[1] = VERTEX_UV_BOTTOM_RIGHT;
            vert_uv[2] = VERTEX_UV_TOP_RIGHT;
            vert_uv[3] = VERTEX_UV_TOP_LEFT;
            break;
        case 1:
            vert_uv[0] = VERTEX_UV_BOTTOM_RIGHT;
            vert_uv[1] = VERTEX_UV_TOP_RIGHT;
            vert_uv[2] = VERTEX_UV_BOTTOM_LEFT;
            vert_uv[3] = VERTEX_UV_TOP_LEFT;
            break;
        case 2:
            vert_uv[0] = VERTEX_UV_BOTTOM_LEFT;
            vert_uv[1] = VERTEX_UV_BOTTOM_RIGHT;
            vert_uv[2] = VERTEX_UV_TOP_RIGHT;
            vert_uv[3] = VERTEX_UV_TOP_LEFT;
            break;
        case 3:
            vert_uv[0] = VERTEX_UV_BOTTOM_LEFT;
            vert_uv[1] = VERTEX_UV_TOP_LEFT;
            vert_uv[2] = VERTEX_UV_BOTTOM_RIGHT;
            vert_uv[3] = VERTEX_UV_TOP_RIGHT;
            break;
        case 4:
            vert_uv[0] = VERTEX_UV_BOTTOM_LEFT;
            vert_uv[1] = VERTEX_UV_BOTTOM_RIGHT;
            vert_uv[2] = VERTEX_UV_TOP_LEFT;
            vert_uv[3] = VERTEX_UV_TOP_RIGHT;
            break;
        case 5:
            vert_uv[0] = VERTEX_UV_BOTTOM_LEFT;
            vert_uv[1] = VERTEX_UV_BOTTOM_RIGHT;
            vert_uv[2] = VERTEX_UV_TOP_LEFT;
            vert_uv[3] = VERTEX_UV_TOP_RIGHT;
            break;
    }

    for (int i = 0; i < 4; i++) {
        verts[(*count)++] = vert_pos[i].x;
        verts[(*count)++] = vert_pos[i].y;
        verts[(*count)++] = vert_pos[i].z;

        // push uvs
        verts[(*count)++] = vert_uv[i].x;
        verts[(*count)++] = vert_uv[i].y;
    }
}

void push_block_indices(int offset, int face, GLuint* indices, int* count) {
    switch (face) {
        case 0:
            indices[(*count)++] = offset + 0;
            indices[(*count)++] = offset + 1;
            indices[(*count)++] = offset + 2;

            indices[(*count)++] = offset + 2;
            indices[(*count)++] = offset + 3;
            indices[(*count)++] = offset + 0;
            break;
        case 1:
            indices[(*count)++] = offset + 0;
            indices[(*count)++] = offset + 2;
            indices[(*count)++] = offset + 3;
        
            indices[(*count)++] = offset + 3;
            indices[(*count)++] = offset + 1;
            indices[(*count)++] = offset + 0;
            break;
        case 2:
            indices[(*count)++] = offset + 3;
            indices[(*count)++] = offset + 2;
            indices[(*count)++] = offset + 1;
        
            indices[(*count)++] = offset + 1;
            indices[(*count)++] = offset + 0;
            indices[(*count)++] = offset + 3;
            break;
        case 3:
            indices[(*count)++] = offset + 2;
            indices[(*count)++] = offset + 0;
            indices[(*count)++] = offset + 1;
        
            indices[(*count)++] = offset + 1;
            indices[(*count)++] = offset + 3;
            indices[(*count)++] = offset + 2;
            break;
        case 4:
            indices[(*count)++] = offset + 2;
            indices[(*count)++] = offset + 3;
            indices[(*count)++] = offset + 1;
        
            indices[(*count)++] = offset + 1;
            indices[(*count)++] = offset + 0;
            indices[(*count)++] = offset + 2;
            break;
        case 5:
            indices[(*count)++] = offset + 1;
            indices[(*count)++] = offset + 0;
            indices[(*count)++] = offset + 2;
        
            indices[(*count)++] = offset + 2;
            indices[(*count)++] = offset + 3;
            indices[(*count)++] = offset + 1;
            break;
    }
}

// deletes data saved to render chunk + frees chunk_t* chunk
void clear_render_chunk(render_chunk_t* rc) {
    free_render_chunk_buffers(rc);
    rc->indices = 0;

    delete_chunk(rc->chunk);
}

void free_render_chunk_buffers(render_chunk_t* rc) {
    glDeleteBuffers(1, &rc->vbo);
    glDeleteBuffers(1, &rc->ebo);
    glDeleteVertexArrays(1, &rc->vao);
}
