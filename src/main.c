#include "game.h"
#include "matrix.h"

int main(char** argv, int argc)
{
    mat4_t mat;
    for (int i = 0; i < 4; i++){
        for (int j = 0;j <4; j++){
            mat.m[i][j] = i + j;
        }
    }

    mat4_print(mat);

    game_init();
    game_loop();
    game_cleanup();

    return 0;
}
