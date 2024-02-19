#ifndef TEST_MATH_H
#define TEST_MATH_H

#include "math/vec3.h"
#include "math/vec2.h"

void test_math_vec3(){
    TESTER_START_GROUP("vec3_t");
    vec3_t a = {1, 1, 1};
    vec3_t b = a;

    vec3_t expected_v = {2, 2, 2};
    vec3_t result_v = vec3_add(a, b);
    
    int r = 1;
    ASSERT_EQUAL_FLOAT(expected_v.x, result_v.x, &r);
    ASSERT_EQUAL_FLOAT(expected_v.y, result_v.y, &r);
    ASSERT_EQUAL_FLOAT(expected_v.z, result_v.z, &r);
    TEST_FINISH(r, "vec3 add");

    r = 1;
    expected_v = (vec3_t){0, 0, 0};
    result_v = vec3_sub(a, b);
    ASSERT_EQUAL_FLOAT(expected_v.x, result_v.x, &r);
    ASSERT_EQUAL_FLOAT(expected_v.y, result_v.y, &r);
    ASSERT_EQUAL_FLOAT(expected_v.z, result_v.z, &r);
    TEST_FINISH(r, "vec3 sub");

    r = 1;
    a = (vec3_t){5.0f, 7.0f, 8.0f};
    float expected_f = 11.74734012447f;
    float result_f = vec3_magnitude(&a);
    ASSERT_EQUAL_FLOAT(expected_f, result_f, &r);
    TEST_FINISH(r, "vec3 magnitude");
    
    TESTER_FINISH_GROUP;
}

void test_math_vec2(){
    TESTER_START_GROUP("vec2_t");

    vec2_t a = {1, 1};
    vec2_t b = a;

    vec2_t expected_v = {2, 2};
    vec2_t result_v = vec2_add(a, b);
    
    int r = 1;
    ASSERT_EQUAL_FLOAT(expected_v.x, result_v.x, &r);
    ASSERT_EQUAL_FLOAT(expected_v.y, result_v.y, &r);
    TEST_FINISH(r, "vec2 add");

    r = 1;
    expected_v = (vec2_t){0, 0};
    result_v = vec2_sub(a, b);
    ASSERT_EQUAL_FLOAT(expected_v.x, result_v.x, &r);
    ASSERT_EQUAL_FLOAT(expected_v.y, result_v.y, &r);
    TEST_FINISH(r, "vec2 sub");

    r = 1;
    a = (vec2_t){5.0f, 7.0f};
    float expected_f = 8.602325267042f;
    float result_f = vec2_magnitude(&a);
    ASSERT_EQUAL_FLOAT(expected_f, result_f, &r);
    TEST_FINISH(r, "vec2 magnitude");

    TESTER_FINISH_GROUP;
}

void test_math() {
    test_math_vec3();
    test_math_vec2();
}

#endif