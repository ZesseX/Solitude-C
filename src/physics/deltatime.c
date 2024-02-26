#include "deltatime.h"

static double deltatime = 0;

void update_deltatime(double dt) {
    deltatime = dt;
}

double get_deltatime() {
    return deltatime;
}

