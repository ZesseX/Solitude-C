#include "debug_hp_timer.h"

static LARGE_INTEGER debug_hp_timer_tps = { .QuadPart = 1 };
static int64_t debug_hp_timer_ticks_per_us = 1;

void debug_hp_timer_init() {
    QueryPerformanceFrequency(&debug_hp_timer_tps);
    debug_hp_timer_ticks_per_us = debug_hp_timer_tps.QuadPart / 1000000;
}

debug_hp_timer_t debug_hp_timer_start() {
    debug_hp_timer_t timer;
    QueryPerformanceCounter(&timer.start);
    return timer;
}

int64_t debug_hp_timer_end(debug_hp_timer_t* timer) {
    QueryPerformanceCounter(&timer->end);
    return (timer->end.QuadPart - timer->start.QuadPart) / debug_hp_timer_ticks_per_us;
}

void debug_hp_timer_print_result(int64_t time) {
    int64_t time_ms = time / 1000;
    int64_t time_s = time_ms / 1000;
    
    printf("timer val:\n\t");
    printf("%" PRId64, time);
    printf("us\n");
    if (time >= 1000) {
        printf("\t%" PRId64, time_ms);
        printf("ms\n");
    }
    else if (time_ms >= 1000) {
        printf("\t%" PRId64, time_s);
        printf("s\n");
    }
    printf("===\n");
}
