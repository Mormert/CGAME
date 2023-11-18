
#ifndef CGAME_ENEMY_H
#define CGAME_ENEMY_H

typedef enum {
    TooSmall,
    Eatable,
    TooLarge
} EnemyState;

typedef struct {
    int dead;
    float x, y;
    float radius;

    float dx, dy;

    EnemyState state;
} Enemy;

void enemy_update(Enemy* enemy, double dt);

#endif //CGAME_ENEMY_H
