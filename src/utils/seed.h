#ifndef SEED_H
#define SEED_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

void set_seed(uint32_t seed);
uint32_t get_seed();

#endif
