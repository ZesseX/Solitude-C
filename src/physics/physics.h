#ifndef PHYSICS_H
#define PHYSICS_H

#include "deltatime.h"
#include "../transform.h"
#include "../math/vec3.h"
#include "../world/world.h"

void physics_frame_start_update();

vec3_t raycast_blocks(vec3_t a, vec3_t b);

#endif
