#include "input.h"

vec3_t input_get_movement(GLFWwindow* window) {
    vec3_t inputs = {0, 0, 0};

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

    return inputs;
}

vec2_t input_get_rotation(GLFWwindow* window) {
    vec2_t inputs = {0, 0};

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

    return inputs;
}
