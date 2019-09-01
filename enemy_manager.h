#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "SDL.h"
#include "util.h"
#include "enemy.h"

struct EnemyManager {
    uint32_t *rand_state;
    struct Enemy *enemies;
    int n;
    float time;
    float spacing;
};

void enemy_manager_init(struct EnemyManager *self, uint32_t *rand_state, struct Enemy *enemies, int n, int w, int h, int v);
struct Enemy *enemy_manager_get_free(struct EnemyManager *self);
void enemy_manager_spawn(struct EnemyManager *self);
void enemy_manager_update(struct EnemyManager *self);
void enemy_manager_render(struct EnemyManager *self, SDL_Renderer *renderer);

#endif
