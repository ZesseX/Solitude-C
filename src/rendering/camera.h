#ifndef CAMERA_H
#define CAMERA_H

#include "../math/matrix.h"
#include "../transform.h"
#include "../math/vec2.h"

#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#define CAMERA_ROTATE_SPEED 0.5f
#define CAMERA_MOVEMENT_SPEED 5

typedef struct {
    mat4_t view;
    mat4_t proj;

    transform_t transform;
} camera_t;

void camera_update_view_matrix(camera_t* camera, transform_t* transform);
void camera_update_proj_matrix(camera_t* camera, float aspect_ratio);

void camera_update_transform_freecam(camera_t* camera, float dt, vec2_t move_inputs, vec2_t rotate_inputs);

#endif
