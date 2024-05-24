#ifndef DEBUG_HP_TIMER_H
#define DEBUG_HP_TIMER_H

#include <Windows.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>

typedef struct {
    LARGE_INTEGER start;
    LARGE_INTEGER end;
} debug_hp_timer_t;

void debug_hp_timer_init();
debug_hp_timer_t debug_hp_timer_start();

int64_t debug_hp_timer_end(debug_hp_timer_t* timer);

void debug_hp_timer_print_result(int64_t time);

#endif
