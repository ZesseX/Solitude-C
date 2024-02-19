#ifndef NOISE_H
#define NOISE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "seed.h"
#include "../math/vec2.h"
#include "../math/math_utils.h"

int noise_from_permutation(int x, int y);
float linear_interpolate(float x, float y, float s);
float smooth_interpolate(float x, float y, float s);
float perlin_noise_per_depth(float x, float y);
float perlin_noise(float x, float y, float freq, int depth);


#endif