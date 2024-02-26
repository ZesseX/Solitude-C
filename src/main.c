#include "game.h"

#define DEBUG 1

int main(char** argv, int argc)
{
    game_init();
    game_loop();
    game_cleanup();

    return 0;
}