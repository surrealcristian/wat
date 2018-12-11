#include "enemy_manager.h"

//TODO: enemy.c start

void enemy_manager_init(
    int   idx,
    float x,
    float y,
    int   w,
    int   h,
    int   v
) {
    int eid  = ENEMY_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];
    int srid = PHYSICS_SDL_RECT_IDX[pid];

    PHYSICS[pid].w = w;
    PHYSICS[pid].h = h;

    PHYSICS_SDL_RECTS[srid].w = PHYSICS[pid].w;
    PHYSICS_SDL_RECTS[srid].h = PHYSICS[pid].h;

    enemy_manager_set_x(idx, x);
    enemy_manager_set_y(idx, y);

    PHYSICS[pid].v = v;

    PHYSICS[pid].alive = 0;
}

void enemy_manager_set_x(
    int   idx,
    float value
) {
    int eid  = ENEMY_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];
    int srid = PHYSICS_SDL_RECT_IDX[pid];

    float x_min = 0 - PHYSICS[pid].w + 1;
    float x_max = WINDOW_W + PHYSICS[pid].w + 1;

    if (value < x_min || value > x_max) {
        PHYSICS[pid].alive = 0;
        return;
    }

    PHYSICS[pid].x = value;

    PHYSICS_SDL_RECTS[srid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
}

void enemy_manager_set_y(
    int   idx,
    float value
) {
    int eid  = ENEMY_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];
    int srid = PHYSICS_SDL_RECT_IDX[pid];

    float y_min = 0 - PHYSICS[pid].h - 1;
    float y_max = WINDOW_H + PHYSICS[pid].h + 1;

    if (value < y_min || value > y_max) {
        PHYSICS[pid].alive = 0;
        return;
    }

    PHYSICS[pid].y = value;

    PHYSICS_SDL_RECTS[srid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
}

void enemy_manager_update(
    int idx
) {
    int eid = ENEMY_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    enemy_manager_set_x(idx, PHYSICS[pid].x + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vx / UPDATES_PER_SECOND));
    enemy_manager_set_y(idx, PHYSICS[pid].y + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vy / UPDATES_PER_SECOND));
}

void enemy_manager_render(
    int          idx,
    SDL_Renderer *renderer
) {
    int eid  = ENEMY_ENTITY_IDX[idx];
    int gid  = ENTITY_GRAPHICS_IDX[eid];
    int srid = GRAPHICS_SDL_RECT_IDX[gid];

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &GRAPHICS_SDL_RECTS[srid]);
}

//TODO: enemy.c end


void enemy_manager_init_all(
    struct EnemyManager *self,
    tinymt32_t          *rand_state,
    int                 w,
    int                 h,
    int                 v
) {
    self->rand_state = rand_state;
    self->n          = n;

    self->time    = 0.0;
    self->spacing = 100.0;

    for (int i = 0; i < ENEMIES_MAX; i++) {
        enemy_manager_init(i, 0, 0, w, h, v);
    }
}

int enemy_manager_get_free(
    struct EnemyManager *self
) {
    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eid = ENEMY_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 1) {
            continue;
        }

        return i;
    }

    return -1;
}

void enemy_manager_spawn(
    struct EnemyManager *self
) {
    int idx = enemy_manager_get_free(self);

    if (idx == -1) {
        return;
    }

    int eid = ENEMY_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    PHYSICS[pid].alive = 1;

    enemy_manager_set_x(idx, rand_n(self->rand_state, WINDOW_W + 1));
    enemy_manager_set_y(idx, 0 - PHYSICS[pid].h);

    PHYSICS[pid].vx = ENEMY_VX;
    PHYSICS[pid].vy = ENEMY_VY;
}

void enemy_manager_update_all(
    struct EnemyManager *self
) {
    for (int i = 0; i < ENEMY_MAX; i++) {
        int eid = ENEMY_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 0) {
            continue;
        }

        enemy_manager_update(i);
    }

    self->time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments

    if (self->time >= self->spacing) {
        enemy_manager_spawn(self);
        self->time = 0.0;
    }
}

void enemy_manager_render_all(
    struct EnemyManager *self,
    SDL_Renderer        *renderer
) {
    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eid = ENEMY_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 0) {
            continue;
        }

        enemy_manager_render(i, renderer);
    }
}
