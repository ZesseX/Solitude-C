#include "physics.h"

static double deltatime;

void physics_frame_start_update() {
    // TODO: Update deltatime
    update_deltatime(0);
}

vec3_t raycast_blocks(vec3_t a, vec3_t b) {
    chunk_t* chunk;
    
    while(1) {
        return (vec3_t){0.0f, 0.0f, 0.0f};
    }
}
