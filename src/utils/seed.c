#include "seed.h"

static uint32_t game_seed = 0;

void set_seed(uint32_t seed) {
    game_seed = seed;
    if (game_seed == 0) {
        time_t t = time(0);
        srand((unsigned)time(&t));
        game_seed = rand();
    }
}

uint32_t get_seed() {
    if (game_seed == 0) {
        set_seed(0);
        printf("Tried to get the seed when it was set to 0. Set it to a new one: %u\n", game_seed);
    }
    return game_seed;
}
