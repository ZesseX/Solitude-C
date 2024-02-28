#ifndef RENDER_H
#define RENDER_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../math/matrix.h"
#include "../transform.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "../input/input.h"

#include "../world/world.h"
#include "../world/chunk.h"
#include "render_chunk.h"
#include "render_debug.h"

typedef struct {
    GLuint vbo;
    GLuint vao;
    GLuint ebo;

    GLuint shader_id;

    GLuint texture_id;

    GLuint texture_loc;
    GLuint mvp_matrix_loc;
} render_context_t;

int renderer_init();
int renderer_update();

void render_update_start();
void render_update_end();

void render_init_temp_chunk();
void render_chunk(chunk_t* chunk);

#endif