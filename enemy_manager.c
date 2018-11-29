#include "enemy_manager.h"

void enemy_manager_init(
    struct EnemyManager *self,
    tinymt32_t          *rand_state,
    struct Enemy        *enemies,
    int                 n,
    int                 w,
    int                 h,
    int                 v
) {
    self->rand_state = rand_state;
    self->enemies    = enemies;
    self->n          = n;

    self->time    = 0.0;
    self->spacing = 1000.0;

    for (int i = 0; i < self->n; i++) {
        enemy_init(&self->enemies[i], 0, 0, w, h, v);
    }
}

struct Enemy *enemy_manager_get_free(
    struct EnemyManager *self
) {
    struct Enemy *enemy = NULL;

    for (int i = 0; i < self->n; i++) {
        if (self->enemies[i].alive == 1) {
            continue;
        }

        enemy = &self->enemies[i];

        break;
    }

    return enemy;
}

void enemy_manager_spawn(
    struct EnemyManager *self
) {
    struct Enemy *enemy = enemy_manager_get_free(self);

    if (enemy == NULL) {
        return;
    }

    enemy->alive = 1;

    enemy_set_x(enemy, rand_n(self->rand_state, WINDOW_W + 1));
    enemy_set_y(enemy, 0 - enemy->h);

    enemy->vx = ENEMY_VX;
    enemy->vy = ENEMY_VY;
}

void enemy_manager_update(
    struct EnemyManager *self
) {
    for (int i = 0; i < self->n; i++) {
        if (self->enemies[i].alive == 0) {
            continue;
        }

        enemy_update(&self->enemies[i]);
    }

    self->time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments

    if (self->time >= self->spacing) {
        enemy_manager_spawn(self);
        self->time = 0.0;
    }
}

void enemy_manager_render(
    struct EnemyManager *self,
    SDL_Renderer        *renderer
) {
    for (int i = 0; i < self->n; i++) {
        if (self->enemies[i].alive == 0) {
            continue;
        }

        enemy_render(&self->enemies[i], renderer);
    }
}
