#ifndef SHADER_H
#define SHADER_H

#include <stdlib.h>
#include <stdio.h>
#include <glad.h>
#include <GLFW/glfw3.h>

typedef struct {
    GLuint id;
} shader_t;

shader_t create_shader_program(const char* vert_path, const char* frag_path, int* success);

static char* shader_load_file(const char* path);
GLuint shader_load(const char* vert_path, const char* frag_path, int* success);

#endif