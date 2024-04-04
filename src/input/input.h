#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include "../math/vec3.h"
#include "../math/vec2.h"

vec3_t input_get_movement(GLFWwindow* window);
vec2_t input_get_rotation(GLFWwindow* window);

#endif
