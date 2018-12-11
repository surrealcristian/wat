#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "SDL.h"
#include "util.h"
#include "enemy.h"

//TODO: enemy.h start

void enemy_manager_init(
    int   idx,
    float x,
    float y,
    int   w,
    int   h,
    int   v
);

void enemy_manager_set_x(
    int   idx,
    float value
);

void enemy_manager_set_y(
    int   idx,
    float value
);


void enemy_manager_update(
    int idx
);

void enemy_manager_render(
    int          idx,
    SDL_Renderer *renderer
);

//TODO: enemy.h end


struct EnemyManager {
    tinymt32_t   *rand_state;
    struct Enemy *enemies;
    int          n;
    float        time;
    float        spacing;
};

void enemy_manager_init_all(
    struct EnemyManager *self,
    tinymt32_t          *rand_state,
    int                 n,
    int                 w,
    int                 h,
    int                 v
);


int enemy_manager_get_free(
    struct EnemyManager *self
);

void enemy_manager_spawn(
    struct EnemyManager *self
);

void enemy_manager_update_all(
    struct EnemyManager *self
);

void enemy_manager_render_all(
    struct EnemyManager *self,
    SDL_Renderer        *renderer
);

#endif
