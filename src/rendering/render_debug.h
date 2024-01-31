#ifndef RENDER_DEBUG_H
#define RENDER_DEBUG_H

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void APIENTRY GLDebugMessageCallback(
    GLenum source, GLenum type, GLuint id, GLenum severity, 
    GLsizei length, const GLchar *msg, const void *data);

#endif