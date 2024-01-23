#include "camera.h"

void camera_update_view_matrix(camera_t* camera) {
    mat4_t translation = mat4_translate(camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);
    mat4_t rotation = mat4_rotate(camera->transform.rotation.x, camera->transform.rotation.y, camera->transform.rotation.z);
    mat4_t dir = mat4_mul(translation, rotation);

    camera->view = dir; //mat4_inverse(&dir);
}

void camera_update_proj_matrix(camera_t* camera, float aspect_ratio) {
    camera->proj = mat4_perspective(CAMERA_FOV, aspect_ratio, 0.1f, 256.0f);
}

void camera_update_transform_freecam(camera_t* camera, float dt, vec2_t move_inputs, vec2_t rotate_inputs) {
    camera->transform.rotation.y -= rotate_inputs.x * CAMERA_ROTATE_SPEED * dt;
    camera->transform.rotation.x -= rotate_inputs.y * CAMERA_ROTATE_SPEED * dt;

    if (camera->transform.rotation.y > PI) {
        camera->transform.rotation.y = -PI;
    }
    else if(camera->transform.rotation.y < -PI) {
        camera->transform.rotation.y = PI;
    }

    vec3_t move_dir = {CAMERA_MOVEMENT_SPEED * dt * -move_inputs.x, 0, CAMERA_MOVEMENT_SPEED * dt * move_inputs.y};
    vec3_t add_vector = vec3_add(&camera->transform.position, &move_dir);
    camera->transform.position = add_vector;
    camera->transform.position.y = -10.0f;

    camera_update_view_matrix(camera);
    camera_update_proj_matrix(camera, 800.0f / 600.0f);
}
