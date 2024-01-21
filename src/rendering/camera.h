#ifndef CAMERA_H
#define CAMERA_H

#include "../math/matrix.h"
#include "../transform.h"

typedef struct {
    mat4_t view;
    mat4_t proj;

    transform_t transform;
} camera_t;

void camera_update_view_matrix(camera_t* camera, transform_t* transform);
void camera_update_proj_matrix(camera_t* camera, float aspect_ratio);

#endif
