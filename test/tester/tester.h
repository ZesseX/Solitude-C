#ifndef TESTER_H
#define TESTER_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#define TESTER_PASS 1
#define TESTER_FAIL 0
#define TESTER_FLOAT_EPSILON 1e-6

#define TESTER_INIT tester_init()
#define TESTER_START_GROUP(name) tester_start_group(name);
#define TESTER_FINISH_GROUP tester_finish_group();

#define ASSERT_EQUAL_INT(expected, actual, result) tester_assert_are_equal_int(expected, actual, result)
#define ASSERT_EQUAL_FLOAT(expected, actual, result) tester_assert_are_equal_float(expected, actual, result)

#define ASSERT_TRUE(expected, result) tester_assert_are_equal_int(1, , result)

#define ASSERT_FALSE(expected, result) tester_assert_are_equal_int(0, expected, result)

#define TEST_FINISH(result, test_name) tester_print_test_name(test_name); \
     tester_finish(result);

void tester_init();

void tester_start_group(const char* name);
void tester_finish_group();

void tester_print_test_name(const char* name);
void tester_print_result(int result);
void tester_finish(int result);

static int tester_test_count_increment(int result);

void tester_assert_are_equal_int(int e, int a, int* result);
void tester_assert_are_equal_float(float e, float a, int* result);

#endif