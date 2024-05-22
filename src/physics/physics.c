#include "physics.h"

static double deltatime;

void physics_frame_start_update(void) {
    update_deltatime();
}

void physics_frame_end_update(void) {
    // Might need this some day
}

vec3_t raycast_blocks(vec3_t a, vec3_t b) {
    chunk_t* chunk;
    
    while(1) {
        return (vec3_t){0.0f, 0.0f, 0.0f};
    }
}
