#include "render_chunk.h"

void checkGLError(const char* location) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error at %s: %d\n", location, err);
    }
}

int update_render_chunk(chunk_t* chunk, render_chunk_t* rc) {
    GLfloat* verts = (GLfloat*)malloc(CHUNK_TOTAL_BLOCK_COUNT * (3 * 4 * 6) * sizeof(GLfloat));
    int verts_count = 0;

    GLuint* indices = (GLuint*)malloc(CHUNK_TOTAL_BLOCK_COUNT * (6 * 6) * sizeof(GLuint));
    int indices_count = 0;

    int visible_faces[6];
    vec3_t pos;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int id = x + CHUNK_SIZE * (y + CHUNK_SIZE * z);

                pos.x = x;
                pos.y = y;
                pos.z = z;

                get_block_visible_faces(chunk, pos, visible_faces);
                for (int i = 0; i < 6; i++){
                    if(!visible_faces[i]) continue;

                    push_block_vertices(i, pos, verts, &verts_count);
                    push_block_indices((verts_count / 3) - 4, i, indices, &indices_count);
                }
            }
        }  
    }

    /*
    for (int i = 0; i < CHUNK_TOTAL_BLOCK_COUNT; i++) {
        pos.x = i % CHUNK_SIZE;
        pos.y = (i / CHUNK_SIZE) % CHUNK_SIZE;
        pos.z = i / (CHUNK_SIZE * CHUNK_SIZE);

        get_block_visible_faces(chunk, pos, visible_faces);
        for (int j = 0; j < 6; j++) {
            if(visible_faces[j] == 0) continue;

            push_block_vertices(j, pos, verts, &verts_count);
            push_block_indices(verts_count / 3, j, indices, &indices_count);
        }
    }
    */
    verts = (GLfloat*)realloc(verts, sizeof(GLfloat) * verts_count);
    indices = (GLuint*)realloc(indices, sizeof(GLuint) * indices_count);

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verts_count, verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices_count, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
   
    rc->vao = vao;
    rc->vbo = vbo;
    rc->ebo = ebo;

    rc->indices = indices_count;

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

    for (int i = 0; i < 4; i++) {
        verts[(*count)++] = vert_pos[i].x;
        verts[(*count)++] = vert_pos[i].y;
        verts[(*count)++] = vert_pos[i].z;
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
