#include "noise.h"

// Original permutation by Ken Perlin
static int permutation[] = { 
    151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
    140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
    247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
    57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
    74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
    60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
    65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
    200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
    52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
    207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
    119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
    129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
    218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
    81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
    184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180 };

float perlin_noise(vec2_t pos) {
    int x0 = (int)floorf(pos.x);
    int x1 = x0 + 1;
    int y0 = (int)floorf(pos.y);
    int y1 = y0 + 1;

    float sx = pos.x - (float)x0;
    float sy = pos.y - (float)y0;

    sx += 0.1f;
    sy += 0.1f;

    float n0, n1, ix0, ix1;
    n0 = perlin_dot_grid_gradient((vec2_t){x0, y0}, pos);    
    n1 = perlin_dot_grid_gradient((vec2_t){x1, y0}, pos);    
    ix0 = perlin_interpolate(n0, n1, sx);

    n0 = perlin_dot_grid_gradient((vec2_t){x0, y1}, pos);
    n1 = perlin_dot_grid_gradient((vec2_t){x1, y1}, pos);
    ix1 = perlin_interpolate(n0, n1, sx);

    float value = perlin_interpolate(ix0, ix1, sy);
    return value;
}

static float perlin_interpolate(float a, float b, float w) {
    return (b - a) * w + a;
}

static vec2_t perlin_random_gradient(vec2_t iv) {
    srand(get_seed() + iv.x * PERLIN_NOISE_X_PRIME + iv.y * PERLIN_NOISE_Y_PRIME);
    float r = rand() * (PI / RAND_MAX * 2);
    //printf("%.2f\n", r);
    return (vec2_t){cosf(r), sinf(r)};
}

static float perlin_dot_grid_gradient(vec2_t iv, vec2_t pos) {
    vec2_t gradient = perlin_random_gradient(iv);
    float dx = pos.x - (float)iv.x;
    float dy = pos.y - (float)iv.y;
    return dx * gradient.x + dy * gradient.y;
}