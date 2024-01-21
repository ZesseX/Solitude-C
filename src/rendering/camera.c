#include "camera.h"

void camera_update_view_matrix(camera_t* camera, transform_t* t) {
    mat4_t translation = mat4_translate(t->position.x, t->position.y, t->position.z);
    mat4_t rotation = mat4_rotate(t->rotation.x, t->rotation.y, t->rotation.z);
    mat4_t dir = mat4_mul(rotation, translation);

    camera->view = dir; //mat4_inverse(&dir);
    //camera->view = mat4_look_at(t->position, (vec3_t){0.0f, 0.0f, 0.0f}, (vec3_t){0.0f, 1.0f, 0.0f});
}

void camera_update_proj_matrix(camera_t* camera, float aspect_ratio) {
    camera->proj = mat4_perspective(70.0f, aspect_ratio, 0.1f, 256.0f);
}

int first1 = 1;
void camera_update_transform_freecam(camera_t* camera, float dt, vec2_t move_inputs, vec2_t rotate_inputs) {
    camera->transform.rotation.y -= rotate_inputs.x * CAMERA_ROTATE_SPEED * dt;
    camera->transform.rotation.x -= rotate_inputs.y * CAMERA_ROTATE_SPEED * dt;
    camera->transform.rotation.x = fmax(-90, fmin(90, camera->transform.rotation.x));

    vec3_t move_dir = {CAMERA_MOVEMENT_SPEED * dt * move_inputs.x, 0, CAMERA_MOVEMENT_SPEED * dt * move_inputs.y};
    vec3_t add_vector = vec3_add(&camera->transform.position, &move_dir);
    camera->transform.position = add_vector;

    //vec3_print(move_dir);

    camera_update_view_matrix(camera, &camera->transform);
    camera_update_proj_matrix(camera, 800.0f / 600.0f);
}
