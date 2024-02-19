#include "tester.h"

static HANDLE _console_handle;
static int _tester_test_group_total_test_count = 0;
static int _tester_test_group_passed_test_count = 0;

void tester_init() {
    _console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("Starting tests...\n\n");
}

void tester_start_group(const char* name) {
    printf("Starting: %s\n", name);
}

void tester_finish_group() {
    printf("Ended test group, passed %d/%d: ", _tester_test_group_passed_test_count, _tester_test_group_total_test_count);
    tester_print_result(_tester_test_group_passed_test_count == _tester_test_group_total_test_count);
    printf("\n");

    _tester_test_group_passed_test_count = 0;
    _tester_test_group_total_test_count = 0;
}

void tester_print_test_name(const char* name) {
    printf("%-30s", name);
}

void tester_print_result(int result) {
    if (result == TESTER_PASS){
        SetConsoleTextAttribute(_console_handle, BACKGROUND_GREEN);
        printf("PASS\n");
    }
    else {
        SetConsoleTextAttribute(_console_handle, BACKGROUND_RED);
        printf("FAIL\n");
    }

    SetConsoleTextAttribute(_console_handle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void tester_finish(int result) {
    tester_test_count_increment(result);
    tester_print_result(result);
}

static int tester_test_count_increment(int result) {
    _tester_test_group_total_test_count++;
    _tester_test_group_passed_test_count += (result) ? 1 : 0;
    return result;
}

void tester_assert_are_equal_int(int e, int a, int* result) {
    *result *= (e == a);
}

void tester_assert_are_equal_float(float e, float a, int* result) {
    *result *= (fabs(e - a) < TESTER_FLOAT_EPSILON);
}