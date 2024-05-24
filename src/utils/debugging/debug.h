#ifndef DEBUG_H
#define DEBUG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "debug_hp_timer.h"
#include "../../physics/deltatime.h"

void debug_init();
void debug_update(GLFWwindow* window);

static void debug_update_fps_data_init();
static void debug_update_fps_data(GLFWwindow* window);

#endif
