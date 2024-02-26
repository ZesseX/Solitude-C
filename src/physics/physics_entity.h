#ifndef PHYSICS_ENTINY_H
#define PHYSICS_ENTITY_H

#include "deltatime.h"
#include "../math/vec3.h"

typedef struct {
    vec3_t dimensions;
    vec3_t location;
    vec3_t rotation;
} physics_entity_t;

void physics_entity_move(physics_entity_t entity, vec3_t dir);

#endif