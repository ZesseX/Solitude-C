#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>
#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include "../tga.h"

typedef struct {
    tga_image_t* tga;
    GLuint id;
} texture_t;

texture_t* load_texture(const char* path);

#endif