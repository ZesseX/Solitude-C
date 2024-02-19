#ifndef VEC2_H
#define VEC2_H

#include <stdio.h>
#include <math.h>

typedef struct {
    float x;
    float y;
} vec2_t;

vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);

float vec2_magnitude(vec2_t* v);
void vec2_normalize(vec2_t* v);

void vec2_print(vec2_t v);

#endif