#ifndef TESTER_H
#define TESTER_H

#include <stdio.h>
#include <stdint.h>

#define TESTER_PRINT_RESULT(result) result ? printf("PASS") : printf("FAIL")

#define ASSERT_EQUAL_INT(expected, actual) TESTER_PRINT_RESULT(expected == actual)

#endif