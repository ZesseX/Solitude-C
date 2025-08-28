#include "player.h"

static player_t gs_player;
static GLFWwindow* gs_window;

int player_init_default(GLFWwindow* window) {
    player_t player;
    player.camera.transform.position = (vec3_t){0.0f, 32.0f, 0.0f};
    
    return player_init(player, window);
}

int player_init(player_t player, GLFWwindow* window) {
    gs_player = player;
    if (window == NULL && gs_window == NULL) {
        printf("Unable to initialize GLFWwindow* for player\n");
        return 0;
    }

    gs_window = window;
    return 1;
}

void player_update() {
    vec3_t move_inputs = input_get_movement(gs_window);
    vec2_t rotation_inputs = input_get_rotation(gs_window);

    camera_update_transform_freecam(&gs_player.camera, get_deltatime(), move_inputs, rotation_inputs);
}

player_t* get_player_ptr() {
    return &gs_player;
}
