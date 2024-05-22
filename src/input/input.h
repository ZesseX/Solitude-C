#ifndef INPUT_H
#define INPUT_H

#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "../math/vec3.h"
#include "../math/vec2.h"

// Might be stupid
typedef struct {
    // MOVEMENT
    // ========================================
    int* move_forward;
    int move_forward_count;

    int* move_backwards;
    int move_backwards_count;

    int* move_right;
    int move_right_count;

    int* move_left;
    int move_left_count;

    int* move_up;
    int move_up_count;

    int* move_down;
    int move_down_count;

    // ROTATE
    // ========================================
    int* rotate_up;
    int rotate_up_count;

    int* rotate_down;
    int rotate_down_count;

    int* rotate_right;
    int rotate_right_count;

    int* rotate_left;
    int rotate_left_count;
} input_config_t;

vec3_t input_get_movement(GLFWwindow* window);
vec2_t input_get_rotation(GLFWwindow* window);

int get_key(GLFWwindow* window, int** key_array, int len);

void init_default_input_config(void);

#endif
