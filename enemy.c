
#include "enemy.h"
#include "game.h"
#include "GLFW/glfw3.h"
#include "render.h"

void enemy_collisions(Enemy* enemy){
    for(int i = 0; i < MAX_ENEMIES; i++){
        Enemy* otherEnemy = &gGame.enemies.enemies[i];
        if(otherEnemy == enemy){
            continue;
        }
        if(otherEnemy->dead){
            continue;
        }
        vec2 diff;
        vec2 pXY = {enemy->x, enemy->y};
        vec2 eXY = {otherEnemy->x, otherEnemy->y};
        vec2_sub(diff, pXY, eXY);
        float areaEnemy = M_PI * enemy->radius * enemy->radius;
        float areaEnemyOther = M_PI * otherEnemy->radius * otherEnemy->radius;
        if(otherEnemy->state == Eatable && vec2_len(diff) < enemy->radius){
            float newAreaPlayer = areaEnemy + areaEnemyOther;
            float newRadius = sqrtf(newAreaPlayer / M_PI);
            enemy->radius = newRadius;
            otherEnemy->dead = 1;
            game_spawn_enemy();
        }
        if(otherEnemy->state == TooSmall && vec2_len(diff) < enemy->radius){
            float newAreaPlayer = areaEnemy - areaEnemyOther;
            float newRadius = sqrtf(newAreaPlayer / M_PI);
            enemy->radius = newRadius;
            otherEnemy->dead = 1;
            game_spawn_enemy();
        }
    }
}

void enemy_update(Enemy* enemy, double dt) {

    if(enemy->dead){
        return;
    }

    Player* player = &gGame.player;
    if(player->radius < enemy->radius - 5.0f){
        enemy->state = TooLarge;
    }else if(player->radius > enemy->radius + 5.0f){
        enemy->state = TooSmall;
    }else{
        enemy->state = Eatable;
    }

    enemy->x += enemy->dx;
    enemy->y += enemy->dy;

    if(fabsf(enemy->x) > 1500){
        enemy->dx *= -1.f;
    }

    if(fabsf(enemy->y) > 1500){
        enemy->dy *= -1.f;
    }

    if(enemy->radius < 25.f || isnan(enemy->radius)){
        enemy->radius = 25.f;
    }

    enemy_collisions(enemy);

    mat4x4 M;
    mat4x4_identity(M);
    mat4x4_translate_in_place(M, enemy->x, enemy->y, 0.f);
    mat4x4_scale_aniso(M, M, enemy->radius, enemy->radius, 1.f);

    shader_use(gGame.circleShaderPlayer);
    shader_set_float(gGame.circleShaderPlayer, "time", glfwGetTime());
    vec3 color = {0.1f, 0.8f, 0.2f};;
    if(enemy->state == TooLarge){
        color[0] = 0.8f;
        color[1] = 0.2f;
        color[2] = 0.1f;
    }else if(enemy ->state == TooSmall){
        color[0] = 0.8f;
        color[1] = 0.5f;
        color[2] = 0.1f;
    }
    shader_set_vec3(gGame.circleShaderPlayer, "color", color);

    render_quad(&M, gGame.circleShaderPlayer);


}