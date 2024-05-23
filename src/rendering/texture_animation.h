#ifndef TEXTURE_ANIMATION_H
#define TEXTURE_ANIMATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture.h"
#include "../physics/deltatime.h"

typedef struct {
    texture_t** textures;
    int texture_count;

    // TODO: Should all textures use the same 'anim_frame_time' and be in sync?
    double anim_frame_time; // Target time for each frame
    double cur_anim_frame_time;
    int cur_anim_frame_index;
} texture_animation_t;

void texture_animation_update(texture_animation_t* texture_anim);
int texture_animations_load_textures(texture_animation_t* texture_anim, int len_textures, const char* name);

GLuint texture_animation_get_current_id(texture_animation_t* texture_anim);

#endif
