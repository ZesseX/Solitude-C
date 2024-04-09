#ifndef CAMERA_H
#define CAMERA_H

#include "../math/matrix.h"
#include "../transform.h"
#include "../math/vec3.h"
#include "../math/vec2.h"
#include "../math/math_utils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define CAMERA_ROTATE_SPEED 12
#define CAMERA_MOVEMENT_SPEED 100
#define CAMERA_FOV 70

typedef struct {
    mat4_t view;
    mat4_t proj;

    transform_t transform;
} camera_t;

void camera_update_view_matrix(camera_t* camera);
void camera_update_proj_matrix(camera_t* camera, float aspect_ratio);

void camera_update_transform_freecam(camera_t* camera, float dt, vec3_t move_inputs, vec2_t rotate_inputs);

#endif
