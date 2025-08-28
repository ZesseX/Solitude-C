#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../input/input.h"
#include "../rendering/camera.h"
#include "../physics/deltatime.h"

typedef struct {
    camera_t camera;
} player_t;

int player_init_default(GLFWwindow* window);
int player_init(player_t player, GLFWwindow* window);

void player_update();
player_t* get_player_ptr();

#endif
