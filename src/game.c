#include "game.h"

void game_init() {
    world_octrees_init_empty();

    renderer_init();
}

void game_loop(){
    int should_close = 0;

    while(!should_close){
        should_close = renderer_update();
    }
}

void game_cleanup() {
    printf("Game cleanup\n");
}
