#include "vec2.h"

vec2_t vec2_add(vec2_t a, vec2_t b) {
    return (vec2_t){a.x + b.x, a.y + b.y};
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
    return (vec2_t){a.x - b.x, a.y - b.y};
}

float vec2_magnitude(vec2_t* v) {
    return sqrtf(v->x * v->x + v->y * v->y);
}

void vec2_normalize(vec2_t* v) {
    if(v->x == 0 && v->y == 0) {
        fprintf(stderr, "Unable to normalize a zero vector\n");
        return;
    }

    float d = sqrtf(v->x + v->y);
    v->x /= d;
    v->y /= d;
}

void vec2_print(vec2_t v) {
    printf("\t[%.2f, %.2f]\n", v.x, v.y);
}
