#include "texture_animation.h"

void texture_animation_update(texture_animation_t* texture_anim) {
    double dt = get_deltatime();
    texture_anim->cur_anim_frame_time += dt;

    double remainder = texture_anim->anim_frame_time - texture_anim->cur_anim_frame_time;
    if (remainder > 0.0) return;

    // add the remainder to the cur_anim_frame_time to avoid flickering / desync with higher framerates
    texture_anim->cur_anim_frame_time = -remainder;
    if (++texture_anim->cur_anim_frame_index >= texture_anim->texture_count) {
        texture_anim->cur_anim_frame_index = 0;
    }
}

int texture_animations_load_textures(texture_animation_t* texture_anim, int len_textures, const char* name) {
    texture_anim->name = (char*)(name);
    
    texture_anim->texture_count = len_textures;
    texture_anim->textures = (texture_t**)malloc(sizeof(texture_t*) * len_textures);

    int len_name = strlen(name);

    // 100 frames allowed in animation 00-99, append _XX to 'name'
    char* name_appended = (char*)malloc((len_name + 4 + 3) * sizeof(char));
    if (name_appended == NULL) {
        return 0;
    }

    strcpy(name_appended, name);

    name_appended[len_name] = '_';

    // TODO: maybe dont hardcode if other file formats supported in the future
    name_appended[len_name + 3] = '.';
    name_appended[len_name + 4] = 't';
    name_appended[len_name + 5] = 'g';
    name_appended[len_name + 6] = 'a';
    name_appended[len_name + 7] = '\0';

    int digit1 = 0, digit2 = 0;
    for (int i = 0; i < texture_anim->texture_count; i++) {
        if (++digit2 == 10) {
            digit1++;
            digit2 = 0;
        }

        name_appended[len_name + 1] = digit1 + '0';
        name_appended[len_name + 2] = digit2 + '0';

        texture_anim->textures[i] = load_texture(name_appended);
    }

    return 1;
}

int texture_animations_reload_textures(texture_animation_t* texture_anim, int len_textures) {
    for (int i = 0; i < texture_anim->texture_count; i++) {
        texture_animations_unload_textures(texture_anim);
        texture_animations_load_textures(texture_anim, texture_anim->texture_count, (const char*)texture_anim->name);
    }
}

int texture_animations_unload_textures(texture_animation_t* texture_anim) {
    for (int i = 0; i < texture_anim->texture_count; i++) {
        unload_texture(texture_anim->textures[i]);
    }
}

inline GLuint texture_animation_get_current_id(texture_animation_t* texture_anim) {
    return texture_anim->textures[texture_anim->cur_anim_frame_index]->id;
}
