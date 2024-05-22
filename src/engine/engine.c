#include "engine.h"

render_context_t render_context;

void engine_init() {
    world_octree_init_empty();

    renderer_init(&render_context);
    physics_frame_start_update();
}

exit_code engine_run() {
    int should_close = 0;
    while(!should_close){
        physics_frame_start_update();
        should_close = renderer_update(&render_context);
    }
    // TODO: should return something useful
    return exit_code_other;
}
