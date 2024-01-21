#ifndef RENDER_H
#define RENDER_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#include "../math/matrix.h"
#include "../transform.h"
#include "camera.h"
#include "../tga.h"
#include "shader.h"

#include "../chunk.h"

#include "../noise.h"

typedef struct {
    GLuint vbo;
    GLuint vao;
    GLuint ebo;

    GLuint shader_id;

    GLuint texture_id;

    GLuint texture_loc;
    GLuint matrix_view_loc;
    GLuint matrix_projection_loc;
    GLuint matrix_model_loc;
} render_block_t;

int renderer_init();
int renderer_update();

void render_update_start();
void render_update_end();

void render_init_block();
void render_chunk(chunk_t* chunk);

#endif