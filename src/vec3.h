#ifndef VEC3_H
#define VEC3_H

#include <math.h>

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

void vec3_normalize(vec3_t* v);
vec3_t vec3_cross(vec3_t a, vec3_t b);
float vec3_dot(vec3_t a, vec3_t b);

#endif