#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "matrix.h"
#include "transform.h"

int renderer_init();

int renderer_update();

void render_block();

static char* shader_load_file(const char* path);
GLuint shader_load(const char* vert_path, const char* frag_path, int* success);

#endif