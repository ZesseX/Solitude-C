#include "debug.h"

#define DEBUG_FPS_DATA_COUNT 5

typedef struct {
    double update_interval;
    double cur_update_interval;
    
    int* fps;
    int fps_index;
} debug_fps_data_t;

static int debug_mode_enabled = 0;
static debug_fps_data_t debug_fps_data;

void debug_init() {
    debug_mode_enabled = 1;
    debug_hp_timer_init();

    debug_update_fps_data_init();
}

// TODO: Just give this file the window info in init
void debug_update(GLFWwindow* window) {
    if (!debug_mode_enabled) return; // should not be called if not enabled but just in case return

    debug_update_fps_data(window);
}

static inline void debug_update_fps_data_init() {
    debug_fps_data.update_interval = (0.1 / DEBUG_FPS_DATA_COUNT);
    debug_fps_data.fps = (int*)malloc(sizeof(int) * DEBUG_FPS_DATA_COUNT);
}

static inline void debug_update_fps_data(GLFWwindow* window) {
    double dt = get_deltatime();
    debug_fps_data.cur_update_interval += dt;

    double remainder = debug_fps_data.update_interval - debug_fps_data.cur_update_interval;
    if (remainder > 0.0) return;

    debug_fps_data.cur_update_interval = -remainder;
    debug_fps_data.fps[debug_fps_data.fps_index] = (int)(1.0 / dt);

    if (++debug_fps_data.fps_index < DEBUG_FPS_DATA_COUNT) return;

    debug_fps_data.fps_index = 0;
    int fps_average = 0;
    for (int i = 0; i < DEBUG_FPS_DATA_COUNT; i++) {
        fps_average += debug_fps_data.fps[i];
    }
    fps_average /= DEBUG_FPS_DATA_COUNT;

    char* fps_text = (char*)malloc(sizeof(char) * 32);
    snprintf(fps_text, 32, "%d", fps_average);
    glfwSetWindowTitle(window, fps_text);
}