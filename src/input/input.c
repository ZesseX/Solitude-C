#include "input.h"

// TODO: Change from global
input_config_t input_config;

vec3_t input_get_movement(GLFWwindow* window) {
    vec3_t inputs = {0, 0, 0};

    inputs.z += get_key(window, &input_config.move_forward, input_config.move_forward_count);
    inputs.z -= get_key(window, &input_config.move_backwards, input_config.move_backwards_count);

    inputs.x += get_key(window, &input_config.move_right, input_config.move_right_count);
    inputs.x -= get_key(window, &input_config.move_left, input_config.move_left_count);

    inputs.y += get_key(window, &input_config.move_up, input_config.move_up_count);
    inputs.y -= get_key(window, &input_config.move_down, input_config.move_down_count);

    /*
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        inputs.z += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        inputs.z -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        inputs.x += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        inputs.x -= 1;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        inputs.y -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        inputs.y += 1;
    }
    */

    return inputs;
}

vec2_t input_get_rotation(GLFWwindow* window) {
    vec2_t inputs = {0, 0};

    inputs.y += get_key(window, &input_config.rotate_up, input_config.rotate_up_count);
    inputs.y -= get_key(window, &input_config.rotate_down, input_config.rotate_down_count);

    inputs.x += get_key(window, &input_config.rotate_right, input_config.rotate_right_count);
    inputs.x -= get_key(window, &input_config.rotate_left, input_config.rotate_left_count);

    /*
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        inputs.y += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        inputs.y -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        inputs.x += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        inputs.x -= 1;
    }
    */

    return inputs;
}

int get_key(GLFWwindow* window, int** key_array, int len) {
    for (int i = 0; i < len; i++) {
        if (glfwGetKey(window, (*key_array)[i]) == GLFW_PRESS) return 1;
    }
    return 0;
}

void init_default_input_config(void) {
    // MOVEMENT
    input_config.move_forward = (int*)malloc(sizeof(int) * 1);
    input_config.move_forward[0] = GLFW_KEY_W;
    input_config.move_forward_count = 1;

    input_config.move_backwards = (int*)malloc(sizeof(int) * 1);
    input_config.move_backwards[0] = GLFW_KEY_S;
    input_config.move_backwards_count = 1;

    input_config.move_right = (int*)malloc(sizeof(int) * 1);
    input_config.move_right[0] = GLFW_KEY_D;
    input_config.move_right_count = 1;

    input_config.move_left = (int*)malloc(sizeof(int) * 1);
    input_config.move_left[0] = GLFW_KEY_A;
    input_config.move_left_count = 1;

    input_config.move_up = (int*)malloc(sizeof(int) * 1);
    input_config.move_up[0] = GLFW_KEY_SPACE;
    input_config.move_up_count = 1;

    input_config.move_down = (int*)malloc(sizeof(int) * 2);
    input_config.move_down[0] = GLFW_KEY_LEFT_SHIFT;
    input_config.move_down[1] = GLFW_KEY_LEFT_CONTROL;
    input_config.move_down_count = 2;

    // ROTATE
    input_config.rotate_up = (int*)malloc(sizeof(int) * 1);
    input_config.rotate_up[0] = GLFW_KEY_UP;
    input_config.rotate_up_count = 1;

    input_config.rotate_down = (int*)malloc(sizeof(int) * 1);
    input_config.rotate_down[0] = GLFW_KEY_DOWN;
    input_config.rotate_down_count = 1;

    input_config.rotate_right = (int*)malloc(sizeof(int) * 1);
    input_config.rotate_right[0] = GLFW_KEY_RIGHT;
    input_config.rotate_right_count = 1;

    input_config.rotate_left = (int*)malloc(sizeof(int) * 1);
    input_config.rotate_left[0] = GLFW_KEY_LEFT;
    input_config.rotate_left_count = 1;
}