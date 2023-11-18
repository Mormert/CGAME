
#include "player.h"
#include "render.h"

#include "game.h"
#include "engine.h"

#include <GLFW/glfw3.h>

void player_get_mat(Player *player, mat4x4 M) {
    mat4x4_identity(M);
    mat4x4_translate_in_place(M, player->x, player->y, 0.f);
    mat4x4_scale_aniso(M, M, player->radius, player->radius, 1.f);
}

void player_camera_view(Player* player){
    mat4x4 view;
    mat4x4_identity(view);
    mat4x4_translate_in_place(view, player->x, player->y, 0.f);
    mat4x4_invert(view, view);
    render_set_view(&view);
}

void player_update(Player* player, double dt) {
    mat4x4 M;
    player_get_mat(player, M);

    vec3 color = {0.9f, 0.8f, 0.9f};
    shader_use(gGame.circleShaderPlayer);
    shader_set_float(gGame.circleShaderPlayer, "time", glfwGetTime());
    shader_set_vec3(gGame.circleShaderPlayer, "color", color);

    render_quad(&M, gGame.circleShaderPlayer);

    if(glfwGetKey(gEngine.window, 'A')){
        player->dv[0] -= dt * 48.f;
    }

    if(glfwGetKey(gEngine.window, 'D')){
        player->dv[0] += dt * 48.f;
    }

    if(glfwGetKey(gEngine.window, 'W')){
        player->dv[1] += dt * 48.f;
    }

    if(glfwGetKey(gEngine.window, 'S')){
        player->dv[1] -= dt * 48.f;
    }

    if(vec2_len(player->dv) > 5.f){
        vec2_norm(player->dv, player->dv);
        vec2_scale(player->dv, player->dv, 5.f);
    }

    player->x += player->dv[0];
    player->y += player->dv[1];

    player->dv[0] *= 0.9999f;
    player->dv[1] *= 0.9999f;

    player_camera_view(player);

    for(int i = 0; i < MAX_ENEMIES; i++){
        Enemy* e = &gGame.enemies.enemies[i];
        if(e->dead){
            continue;
        }
        vec2 diff;
        vec2 pXY = {player->x, player->y};
        vec2 eXY = {e->x, e->y};
        vec2_sub(diff, pXY, eXY);
        float areaPlayer = M_PI * player->radius * player->radius;
        float areaEnemy = M_PI * e->radius * e->radius;
        if(e->state == Eatable && vec2_len(diff) < player->radius){
            float newAreaPlayer = areaPlayer + areaEnemy;
            float newRadius = sqrtf(newAreaPlayer / M_PI);
            player->radius = newRadius;
            e->dead = 1;
        }
        if(e->state == TooSmall && vec2_len(diff) < player->radius){
            float newAreaPlayer = areaPlayer - areaEnemy;
            float newRadius = sqrtf(newAreaPlayer / M_PI);
            player->radius = newRadius;
            e->dead = 1;
        }
        if(e->state == TooLarge && vec2_len(diff) < player->radius){
            game_destroy();
            game_init();
        }
    }

    player->radius -= dt * 2.f;
    if(player->radius < 25.f || isnan(player->radius)){
        player->radius = 25.f;
    }

}

void player_init(Player *player) {
    player->y = 0.f;
    player->x = 0.f;
    player->radius = 25.f;
    player->dv[0] = 0;
    player->dv[1] = 0;
}
