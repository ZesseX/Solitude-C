#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vec3.h"

typedef struct {
    vec3_t position;
    vec3_t rotation;
    vec3_t scale;
} transform_t;

#endif
