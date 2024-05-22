#ifndef EXIT_REASON_H
#define EXIT_REASON_H

#include <stdio.h>

typedef enum {
    exit_code_other = 0,
} exit_code;

void exit_reason_print(exit_code code);

#endif
