#ifndef ENGINE_H
#define ENGINE_H

#include "exit_reason.h"
#include "../rendering/render.h"
#include "../physics/physics.h"
#include "../player.h"
#include "../world/world.h"

void engine_init();
// returns exit code
exit_code engine_run();

#endif
