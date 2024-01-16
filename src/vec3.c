#include "vec3.h"

void vec3_normalize(vec3_t* v) {
    float d = sqrtf(v->x + v->y + v->z);
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
