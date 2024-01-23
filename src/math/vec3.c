#include "vec3.h"

vec3_t vec3_add(vec3_t* a, vec3_t* b) {
    return (vec3_t){a->x + b->x, a->y + b->y, a->z + b->z};
}

vec3_t vec3_sub(vec3_t* a, vec3_t* b) {
    return (vec3_t){a->x - b->x, a->y - b->y, a->z - b->z};
}

float vec3_magnitude(vec3_t* v) {
    return sqrtf(v->x * v->x + v->y * v->y + v->z + v->z);
}

void vec3_normalize(vec3_t* v) {
    if(v->x == 0 && v->y == 0 && v->z == 0) {
        //fprintf(stderr, "Unable to normalize a zero vector\n");
        return;
    }

    float d = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= d;
    v->y /= d;
    v->z /= d;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    // ((a_y * b_z) - (a_z * b_y) , (a_z * b_x) - (a_x * b_z) , (a_x * b_y) - (a_y * b_x))
    // i                          ,  j                        ,  k
    // [ i ,  j ,  k ]
    // [a.x, a.y, a.z]
    // [b.x, b.y, b.z]

    vec3_t v;
    v.x = (a.y * b.z) - (a.z * b.y);
    v.y = (a.z * b.x) - (a.x * b.z);
    v.z = (a.x * b.y) - (a.y * b.x);
    return v;
}

float vec3_dot(vec3_t a, vec3_t b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void vec3_print(vec3_t v) {
    printf("\t[%.2f, %.2f, %.2f]\n", v.x, v.y, v.z);
}
