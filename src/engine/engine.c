#include "engine.h"

render_context_t render_context;

void engine_init() {
    world_octree_init_empty();

    // TODO: Alternatively load in a config file for inputs so that they are saved between sessions
    init_default_input_config();

    renderer_init(&render_context);
    physics_frame_start_update();
}

exit_code engine_run() {
    int should_close = 0;
    while(!should_close){
        physics_frame_start_update();
        texture_animation_update(&render_context.texture_animation_main_atlas);
        should_close = renderer_update(&render_context);
        debug_update(render_context.window);
    }
    // TODO: should return something useful
    return exit_code_other;
}
