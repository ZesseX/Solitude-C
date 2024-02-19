#include "game.h"

game_world_t game_world;

void game_init() {
    player_t player;

    //init_chunk(game_world.chunk);
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
