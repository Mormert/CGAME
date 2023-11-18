
#include "game.h"

#include "render.h"
#include "enemy.h"

#include <linmath.h>
#include <GLFW/glfw3.h>

Game gGame;

float randomNearby(float value) {
    float offset = 0.4f * value;

    // Generate a random float between -offset and +offset
    float randOffset = (float)rand() / RAND_MAX * 2 * offset - offset;

    // Add the random offset to the original value
    float nearbyValue = value + randOffset;

    return nearbyValue;
}

void game_spawn_enemy() {
    for(int i = 0; i < MAX_ENEMIES; i++){
        Enemy* e = &gGame.enemies.enemies[i];
        if(e->dead){
            e->dead = 0;
            e->radius = randomNearby(gGame.player.radius);
            e->x = rand() % 2501 - 1250;
            e->y = rand() % 2501 - 1250;
            e->dx = rand() % 3 - 2;
            e->dy = rand() % 3 - 2;
            break;
        }
    }
}

void game_init(){
    gGame.quadShader = shader_create("../shaders/quad.vert", "../shaders/quad.frag");
    gGame.circleShader = shader_create("../shaders/quad.vert", "../shaders/circle.frag");
    gGame.circleShaderPlayer = shader_create("../shaders/quad.vert", "../shaders/circle_player.frag");

    player_init(&gGame.player);

    for(int i = 0; i < MAX_ENEMIES; i++){
        gGame.enemies.enemies[i].dead = 1;
    }

    for(int i = 0; i < MAX_ENEMIES; i++){
        game_spawn_enemy();
    }

}

void game_destroy() {
    shader_destroy(gGame.quadShader);
    shader_destroy(gGame.circleShader);
    shader_destroy(gGame.circleShaderPlayer);
}

void game_enemies_update(Enemies* enemies, double dt) {
    for(int i = 0; i < MAX_ENEMIES; i++){
        enemy_update(&enemies->enemies[i], dt);
    }
}

void game_loop(double dt) {
    player_update(&gGame.player, dt);
    game_enemies_update(&gGame.enemies, dt);
}




