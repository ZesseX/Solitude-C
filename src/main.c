#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "engine/engine.h"
#include "utils/debugging/debug.h"

#define DEBUG 1

int main(char** argv, int argc) {
    // TODO: Put debug on or off into args
    if (DEBUG) debug_init();

    engine_init();

    exit_code exit_code = engine_run();
    exit_reason_print(exit_code);
    
    return exit_code;
}
