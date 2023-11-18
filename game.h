
#ifndef CGAME_GAME_H
#define CGAME_GAME_H

#include "shader.h"
#include "player.h"
#include "enemy.h"

#define MAX_ENEMIES 512

typedef struct{
    Enemy enemies[MAX_ENEMIES];
} Enemies;

typedef struct  {
    Shader* quadShader;
    Shader* circleShader;
    Shader* circleShaderPlayer;
    Player player;
    Enemies enemies;
} Game;

extern Game gGame;

void game_init();
void game_destroy();

void game_loop(double dt);

#endif //CGAME_GAME_H
