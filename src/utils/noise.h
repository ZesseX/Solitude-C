#ifndef NOISE_H
#define NOISE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "seed.h"
#include "../math/vec2.h"
#include "../math/math_utils.h"

#define PERLIN_NOISE_X_PRIME 65537
#define PERLIN_NOISE_Y_PRIME 4294967291

float perlin_noise(vec2_t pos);

static float perlin_interpolate(float a, float b, float w);
static vec2_t perlin_random_gradient(vec2_t iv);
static float perlin_dot_grid_gradient(vec2_t iv, vec2_t pos);

#endif