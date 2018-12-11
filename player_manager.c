#include "player_manager.h"

void player_manager_init(
    int   idx,
    float x,
    float y,
    int   w,
    int   h,
    int   v
) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];
    int sid = ENTITY_SHOOTING_IDX[eid];
    int srid = PHYSICS_SDL_RECT_IDX[pid];

    PHYSICS[pid].w = w;
    PHYSICS[pid].h = h;

    PHYSICS_SDL_RECTS[srid].w = PHYSICS[pid].w;
    PHYSICS_SDL_RECTS[srid].h = PHYSICS[pid].h;

    player_manager_set_x(idx, x);
    player_manager_set_y(idx, y);

    PHYSICS[pid].v = v;

    PHYSICS[pid].alive = 1;

    SHOOTING[sid].bullets_n = PLAYER_BULLETS_INIT_N;
    SHOOTING[sid].fire_spacing = 128;
}

void player_manager_set_x(
    int   idx,
    float value
) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];
    int srid = PHYSICS_SDL_RECT_IDX[pid];

    float x_min = 0 + PHYSICS[pid].w / 2;
    float x_max = WINDOW_W - PHYSICS[pid].w / 2;

    if (value < x_min) {
        value = x_min;
    } else if (value > x_max) {
        value = x_max;
    }

    PHYSICS[pid].x = value;

    PHYSICS_SDL_RECTS[srid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
}

void player_manager_set_y(
    int   idx,
    float value
) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];
    int srid = PHYSICS_SDL_RECT_IDX[pid];

    float y_min = 0 + PHYSICS[pid].h / 2;
    float y_max = WINDOW_H - PHYSICS[pid].h / 2;

    if (value < y_min) {
        value = y_min;
    } else if (value > y_max) {
        value = y_max;
    }

    PHYSICS[pid].y = value;

    PHYSICS_SDL_RECTS[srid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
}

void player_manager_on_button_a_keydown(
    int idx
) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int sid = ENTITY_SHOOTING_IDX[eid];

    SHOOTING[sid].firing = 1;
    SHOOTING[sid].fire_time = SHOOTING[sid].fire_spacing * 1.0;
}

void player_manager_on_button_a_keyup(
    int idx
) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int sid = ENTITY_SHOOTING_IDX[eid];

    SHOOTING[sid].firing = 0;
    SHOOTING[sid].fire_time = 0.0;
}

void player_manager_fire(
    int idx
) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];
    int sid = ENTITY_SHOOTING_IDX[eid];

    for (int i = 0; i < SHOOTING[sid].bullets_n; i++) {
        int bid = bullet_manager_get_free();

        if (bid == -1) {
            return;
        }

        int beid = BULLET_ENTITY_IDX[bid];
        int bpid = ENTITY_PHYSICS_IDX[beid];

        PHYSICS[bpid].alive = 1;

        bullet_manager_set_x(bid, PHYSICS[pid].x + PHYSICS[pid].w * PLAYER_BULLETS_OFFSET_X[i]);
        bullet_manager_set_y(bid, PHYSICS[pid].y + PHYSICS[pid].h * PLAYER_BULLETS_OFFSET_Y[i]);

        PHYSICS[bpid].vx = PLAYER_BULLETS_VX;
        PHYSICS[bpid].vy = PLAYER_BULLETS_VY;
    }
}

void player_manager_update(
    int idx
) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];
    int sid = ENTITY_SHOOTING_IDX[eid];

    PHYSICS[pid].vx = 0;
    PHYSICS[pid].vy = 0;

    if (KEYS.right) {
        PHYSICS[pid].vx = +1;
    }

    if (KEYS.left) {
        PHYSICS[pid].vx = -1;
    }

    if (KEYS.up) {
        PHYSICS[pid].vy = -1;
    }

    if (KEYS.down) {
        PHYSICS[pid].vy = +1;
    }

    player_manager_set_x(idx, PHYSICS[pid].x + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vx / UPDATES_PER_SECOND));
    player_manager_set_y(idx, PHYSICS[pid].y + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vy / UPDATES_PER_SECOND));

    if (KEYS.z) {
        SHOOTING[sid].fire_time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments
    }

    if (SHOOTING[sid].fire_time >= SHOOTING[sid].fire_spacing) {
        player_manager_fire(idx);
        SHOOTING[sid].fire_time = 0.0;
    }
}

void player_manager_render(
    int           idx,
    SDL_Renderer  *renderer
) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int gid = ENTITY_GRAPHICS_IDX[eid];
    int srid = GRAPHICS_SDL_RECT_IDX[gid];

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &GRAPHICS_SDL_RECTS[srid]);
}
