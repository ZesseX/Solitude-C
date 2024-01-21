#include "camera.h"

void camera_update_view_matrix(camera_t* camera, transform_t* t) {
    camera->view = mat4_look_at(t->position, (vec3_t){0.0f, 0.0f, 0.0f}, (vec3_t){0.0f, 1.0f, 0.0f});
}

void camera_update_proj_matrix(camera_t* camera, float aspect_ratio) {
    camera->proj = mat4_perspective(70.0f, aspect_ratio, 0.1f, 256.0f);
}
