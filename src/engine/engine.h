#ifndef ENGINE_H
#define ENGINE_H

#include "exit_reason.h"
#include "../rendering/render.h"
#include "../rendering/texture_animation.h"
#include "../physics/physics.h"
#include "../player.h"
#include "../world/world.h"
#include "../input/input.h"

void engine_init();
// returns exit code
exit_code engine_run();

#endif
