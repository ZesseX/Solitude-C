#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <math.h>
#include "../transform.h"

typedef struct {
    float m[4][4];
} mat4_t;

mat4_t mat4_identity();
mat4_t mat4_mul(mat4_t a, mat4_t b);

mat4_t mat4_translate(float x, float y, float z);
mat4_t mat4_rotate(float x, float y, float z);
mat4_t mat4_scale(float x, float y, float z);

mat4_t mat4_rotate_x(float angle);
mat4_t mat4_rotate_y(float angle);
mat4_t mat4_rotate_z(float angle);

mat4_t mat4_model(transform_t* t);
mat4_t mat4_perspective(float fov, float aspect_ratio, float near, float far);
mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up);

mat4_t mat4_inverse(mat4_t* m);

void mat4_print(mat4_t m);

#endif