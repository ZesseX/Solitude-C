#ifndef VEC3_H
#define VEC3_H

#include <stdio.h>
#include <math.h>
#include "math_utils.h"

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

vec3_t vec3_add(vec3_t* a, vec3_t* b);
vec3_t vec3_sub(vec3_t* a, vec3_t* b);

float vec3_magnitude(vec3_t* v);

void vec3_normalize(vec3_t* v);
vec3_t vec3_cross(vec3_t a, vec3_t b);
float vec3_dot(vec3_t a, vec3_t b);

void vec3_print(vec3_t v);

#endif