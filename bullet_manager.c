#include "bullet_manager.h"

//TODO: bullet.c start

void bullet_manager_init(
    int   idx,
    float x,
    float y,
    int   w,
    int   h,
    int   v
) {
    int eid  = BULLET_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];
    int srid = PHYSICS_SDL_RECT_IDX[pid];

    PHYSICS[pid].w = w;
    PHYSICS[pid].h = h;

    PHYSICS_SDL_RECTS[srid].w = PHYSICS[pid].w;
    PHYSICS_SDL_RECTS[srid].h = PHYSICS[pid].h;

    bullet_manager_set_x(idx, x);
    bullet_manager_set_y(idx, y);

    PHYSICS[pid].v = v;

    PHYSICS[pid].alive = 0;
}

float bullet_manager_get_x(
    int idx
) {
    int eid = BULLET_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    return PHYSICS[pid].x;
}

void bullet_manager_set_x(
    int   idx,
    float value
) {
    int eid  = BULLET_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];
    int srid = PHYSICS_SDL_RECT_IDX[pid];

    float x_min = 0 - PHYSICS[pid].w - 1;
    float x_max = WINDOW_W + PHYSICS[pid].w + 1;

    if (value < x_min || value > x_max) {
        PHYSICS[pid].alive = 0;
        return;
    }

    PHYSICS[pid].x = value;

    PHYSICS_SDL_RECTS[srid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
}

float bullet_manager_get_y(
    int idx
) {
    int eid = BULLET_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    return PHYSICS[pid].y;
}

void bullet_manager_set_y(
    int   idx,
    float value
) {
    int eid  = BULLET_ENTITY_IDX[idx];
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

void bullet_manager_update(
    int idx
) {
    bullet_manager_set_x(idx, PHYSICS[pid].x + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vx / UPDATES_PER_SECOND));
    bullet_manager_set_y(idx, PHYSICS[pid].y + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vy / UPDATES_PER_SECOND));
}

void bullet_manager_render(
    int          idx,
    SDL_Renderer *renderer
) {
    int eid  = BULLET_ENTITY_IDX[idx];
    int gid  = ENTITY_GRAPHICS_IDX[eid];
    int srid = GRAPHICS_SDL_RECT_IDX[srid];

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &GRAPHICS_SDL_RECTS[srid]);
}

//TODO: bullet.c end


void bullet_manager_init_all(
    int                  w,
    int                  h,
    int                  v
) {
    for (int i = 0; i < BULLET_MAX; i++) {
        bullet_manager_init(i, 0, 0, w, h, v);
    }
}

int bullet_manager_get_free() {
    for (int i = 0; i < BULLET_MAX; i++) {
        int eid = BULLET_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 1) {
            continue;
        }

        return i;
    }

    return -1;
}

void bullet_manager_update_all() {
    for (int i = 0; i < BULLET_MAX; i++) {
        int eid = BULLET_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 0) {
            continue;
        }

        bullet_manager_update(i);
    }
}

void bullet_manager_render_all(
    SDL_Renderer *renderer
) {
    for (int i = 0; i < BULLET_MAX; i++) {
        int eid = BULLET_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 0) {
            continue;
        }

        bullet_manager_render(i, renderer);
    }
}
