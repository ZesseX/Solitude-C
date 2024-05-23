#include "deltatime.h"

static double last_frame_time = 0;
static double deltatime = 0;

void update_deltatime() {
    double cur_frame_Time = glfwGetTime();
    deltatime = cur_frame_Time - last_frame_time;
    last_frame_time = cur_frame_Time;
}

inline double get_deltatime() {
    return deltatime;
}
