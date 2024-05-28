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
#include <pthread.h>

#include "render_chunk.h"
#include "render_debug.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "texture_animation.h"
#include "../math/matrix.h"
#include "../physics/deltatime.h"
#include "../transform.h"
#include "../input/input.h"
#include "../world/world.h"
#include "../world/chunk.h"
#include "../utils/debugging/debug.h"

typedef struct {
    GLFWwindow* window;

    render_chunk_context_t render_chunk_context;
    render_chunk_t* render_chunks;
    int render_chunk_count;

    texture_animation_t texture_animation_main_atlas;
} render_context_t;

int renderer_init(render_context_t* render_context);
int renderer_update(render_context_t* render_context);

void render_update_start(render_context_t* render_context);
void render_update_end(render_context_t* render_context);

void render_init_render_chunks(render_context_t* render_context);
void render_chunk(render_context_t* rc);

typedef struct {
    chunk_t** chunks;
    render_chunk_t* rcs;

    int* num_verts;
    int* num_indices;

    GLfloat** verts;
    GLuint** indices;

    int start_index;
    int end_index;
} rc_create_thread_data_t;

void* rc_create_func(void* args);

#endif