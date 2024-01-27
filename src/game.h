#ifndef GAME_H
#define GAME_H

#define CHUNK_COUNT_PER_AXIS 2

#include "rendering/render.h"
#include "player.h"

#include "world/chunk.h"

typedef struct {
    chunk_t* chunk;
} game_world_t;

void game_init();
void game_loop();
void game_cleanup();

#endif