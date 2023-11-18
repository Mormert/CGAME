
#ifndef CGAME_PLAYER_H
#define CGAME_PLAYER_H

#include <linmath.h>

typedef struct {
    float x, y;
    float radius;
    vec2 dv;
} Player;

void player_init(Player* player);

void player_update(Player* player, double dt);


#endif //CGAME_PLAYER_H
