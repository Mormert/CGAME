
#ifndef CGAME_GAME_H
#define CGAME_GAME_H

#include "shader.h"
#include "player.h"
#include "blob.h"
#include "grid.h"

#define MAX_BLOBS 300
#define INITIAL_BLOBS 256

typedef struct  {
    Shader* quadShader;
    Shader* circleShader;
    Shader* circleShaderPlayer;

    Grid* grid;

    Blob blobs[MAX_BLOBS];

    int playerBlobIndex;
} Game;

extern Game gGame;

void game_init();
void game_destroy();

void game_loop(double dt);

int game_spawn_blob();

#endif //CGAME_GAME_H
