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

#include "render_chunk.h"
#include "render_debug.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "../math/matrix.h"
#include "../physics/deltatime.h"
#include "../transform.h"
#include "../input/input.h"
#include "../world/world.h"
#include "../world/chunk.h"

typedef struct {
    render_chunk_context_t render_chunk_context;
    render_chunk_t* render_chunks;
    int render_chunk_count;
} render_context_t;

int renderer_init(render_context_t* render_context);
int renderer_update(render_context_t* render_context);

void render_update_start();
void render_update_end();

void render_init_render_chunks(render_context_t* render_context);
void render_chunk(render_context_t* rc);

#endif