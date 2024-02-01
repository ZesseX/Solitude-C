#ifndef RENDER_DEBUG_H
#define RENDER_DEBUG_H

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define OGL_DEBUG_FLAG_HIGH                 (1 << 0)
#define OGL_DEBUG_FLAG_MEDIUM               (1 << 1)
#define OGL_DEBUG_FLAG_LOW                  (1 << 2)
#define OGL_DEBUG_FLAG_NOTIFICATION         (1 << 3)

void opengl_debug_init(const int flags);

void APIENTRY opengl_debug_msg_callback(
    GLenum src, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* msg, const void* data);

#endif