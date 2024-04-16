#include "matrix.h"

mat4_t mat4_identity()
{
    mat4_t m;
    m.m[0][0] = 1.0f;
    m.m[0][1] = 0.0f;
    m.m[0][2] = 0.0f;
    m.m[0][3] = 0.0f;

    m.m[1][0] = 0.0f;
    m.m[1][1] = 1.0f;
    m.m[1][2] = 0.0f;
    m.m[1][3] = 0.0f;

    m.m[2][0] = 0.0f;
    m.m[2][1] = 0.0f;
    m.m[2][2] = 1.0f;
    m.m[2][3] = 0.0f;

    m.m[3][0] = 0.0f;
    m.m[3][1] = 0.0f;
    m.m[3][2] = 0.0f;
    m.m[3][3] = 1.0f;

    return m;
}

mat4_t mat4_mul(mat4_t a, mat4_t b)
{
    mat4_t m;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            m.m[i][j] = 0.0f;
            for (int k = 0; k < 4; k++){
                m.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return m;
}

mat4_t mat4_translate(float x, float y, float z){
    mat4_t m = mat4_identity();
    m.m[3][0] = x;
    m.m[3][1] = y;
    m.m[3][2] = z;
    return m;
}

mat4_t mat4_rotate(float x, float y, float z) {
    mat4_t rx = x == 0 ? mat4_identity() : mat4_rotate_x(x);
    mat4_t ry = y == 0 ? mat4_identity() : mat4_rotate_y(y);
    mat4_t rz = z == 0 ? mat4_identity() : mat4_rotate_z(z);

    mat4_t m = mat4_mul(mat4_mul(rz, ry), rx);
    return m;
}

mat4_t mat4_scale(float x, float y, float z) {
    mat4_t m = mat4_identity();
    m.m[0][0] = x;
    m.m[1][1] = y;
    m.m[2][2] = z;
    return m;
}

mat4_t mat4_rotate_x(float angle){
    mat4_t m = mat4_identity();
    float c = cos(angle);
    float s = sin(angle);

    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;

    return m;
}

mat4_t mat4_rotate_y(float angle){
    mat4_t m = mat4_identity();
    float c = cos(angle);
    float s = sin(angle);

    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;

    return m;
}

mat4_t mat4_rotate_z(float angle){
    mat4_t m = mat4_identity();
    float c = cos(angle);
    float s = sin(angle);

    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;

    return m;
}

mat4_t mat4_model(transform_t* t){
    mat4_t tm = mat4_translate(t->position.x, t->position.y, t->position.z);
    mat4_t rm = mat4_rotate(t->rotation.x, t->rotation.y, t->rotation.z);
    mat4_t sm = mat4_scale(t->scale.x, t->scale.y, t->scale.z);

    mat4_t model = mat4_mul(mat4_mul(tm, rm), sm);

    return model;
}

mat4_t mat4_perspective(float fov, float aspect_ratio, float near, float far){
    // fov in radians, aspect ratio = width / height
    mat4_t m = mat4_identity();
    float f = 1.0f / tan(fov * 0.5f);

    m.m[0][0] = f / aspect_ratio;
    m.m[1][1] = f;
    m.m[2][2] = (far + near) / (near - far);
    m.m[2][3] = -1.0f;
    m.m[3][2] = (2.0f * far * near) / (near - far);
    m.m[3][3] = 0.0f;
    
    return m;
}

mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up){
    vec3_t forward = {
        .x = target.x - eye.x,
        .y = target.y - eye.y,
        .z = target.z - eye.z
    };
    vec3_normalize(&forward);

    vec3_t right = vec3_cross(up, forward);
    vec3_normalize(&right);

    // TODO: If the 'up' vec3 provided is wrong then the right vec3 is also wrong? So this 'new_up' may not help much.
    vec3_t new_up = vec3_cross(forward, right);
    vec3_normalize(&new_up);

    mat4_t m = {
        right.x, new_up.x, -forward.x, 0.0f,
        right.y, new_up.y, -forward.y, 0.0f,
        right.z, new_up.z, -forward.z, 0.0f,
        -vec3_dot(right, eye), -vec3_dot(new_up, eye), vec3_dot(forward, eye), 1.0f
    };

    return m;
}

mat4_t mat4_inverse(mat4_t* m) {
    // https://graphics.stanford.edu/courses/cs248-98-fall/Final/q4.html
    mat4_t r = mat4_identity(); // rotation matrix
    mat4_t t = mat4_identity(); // transform matrix

    r.m[0][0] = m->m[0][0];
    r.m[0][1] = m->m[1][0];
    r.m[0][2] = m->m[2][0];

    r.m[1][0] = m->m[0][1];
    r.m[1][1] = m->m[1][1];
    r.m[1][2] = m->m[2][1];
    
    r.m[2][0] = m->m[0][2];
    r.m[2][1] = m->m[1][2];
    r.m[2][2] = m->m[2][2];

    t.m[3][0] = -m->m[3][0];
    t.m[3][1] = -m->m[3][1];
    t.m[3][2] = -m->m[3][2];

    float ux = t.m[0][0];
    float uy = t.m[0][1];
    float uz = t.m[0][2];

    float vx = t.m[1][0];
    float vy = t.m[1][1];
    float vz = t.m[1][2];

    float wx = t.m[2][0];
    float wy = t.m[2][1];
    float wz = t.m[2][2];

    vec3_t pos = (vec3_t){t.m[3][0], t.m[3][1], t.m[3][2]};
    t.m[3][0] = (-ux * pos.x) - (uy * pos.y) - (uz * pos.z);
    t.m[3][1] = (-vx * pos.x) - (vy * pos.y) - (vz * pos.z);
    t.m[3][2] = (-wx * pos.x) - (wy * pos.y) - (wz * pos.z);
    pos = (vec3_t){t.m[3][0], t.m[3][1], t.m[3][2]};

    /*
    r.m[3][0] = -vec3_dot((vec3_t){r.m[0][0], r.m[0][1], r.m[0][2]}, pos);
    r.m[3][1] = -vec3_dot((vec3_t){r.m[1][0], r.m[1][1], r.m[1][2]}, pos);
    r.m[3][2] = -vec3_dot((vec3_t){r.m[2][0], r.m[2][1], r.m[2][2]}, pos);
    */

    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[3][3] = 1.0f;

    return mat4_mul(t, r);
}

vec3_t mat4_mul_vec3(mat4_t* m, vec3_t v) {
    vec3_t out;
    out.x = m->m[0][0] * v.x + m->m[1][0] * v.y + m->m[2][0] * v.z + m->m[3][0];
    out.y = m->m[0][1] * v.x + m->m[1][1] * v.y + m->m[2][1] * v.z + m->m[3][1];
    out.z = m->m[0][2] * v.x + m->m[1][2] * v.y + m->m[2][2] * v.z + m->m[3][2];
    return out;
}

void mat4_print(mat4_t m){
    printf("Print matrix:\n");
    for (int i = 0; i < 4; i++){
        printf("\t");
        for (int j = 0; j < 4; j++){
            printf("[%.1f]", m.m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
