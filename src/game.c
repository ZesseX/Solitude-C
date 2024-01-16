#include "game.h"

void game_init() {
    renderer_init();
}

void game_loop(){
    int should_close = 0;

    while(!should_close){
        should_close = !renderer_update();
    }
}

void game_cleanup() {

}
