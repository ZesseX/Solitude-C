#include "texture.h"

texture_t* load_texture(const char* path) {
    texture_t* texture = (texture_t*)(malloc(sizeof(texture_t)));
    texture->tga = load_tga(path);
    if(&texture->tga == NULL){
        fprintf(stderr, "Unable to create texture\n");
        return NULL;
    }

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->tga->width, texture->tga->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, texture->tga->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenerateMipmap(texture->id);

    return texture;
}
