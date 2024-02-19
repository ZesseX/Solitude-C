#include <stdio.h>

#include "tester/tester.h"

#include "test_math.h"

int main() {
    TESTER_INIT;

    test_math();

    getchar();

    return 0;
}
