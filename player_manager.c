#include "player_manager.h"

void player_manager_init(
    int   index,
    float x,
    float y,
    int   w,
    int   h,
    int   v
) {
    int eid = PLAYER_ENTITY_INDEX[index];
    int pcid = ENTITY_PHYSICS_COMPONENT_INDEX[eid];
    int scid = ENTITY_SHOOTING_COMPONENT_INDEX[eid];
    int srid = PHYSICS_COMPONENT_SDL_RECT_INDEX[pcid];

    PHYSICS_COMPONENTS[pcid].w = w;
    PHYSICS_COMPONENTS[pcid].h = h;

    PHYSICS_COMPONENT_SDL_RECTS[srid].w = PHYSICS_COMPONENTS[pcid].w;
    PHYSICS_COMPONENT_SDL_RECTS[srid].h = PHYSICS_COMPONENTS[pcid].h;

    player_manager_set_x(index, x);
    player_manager_set_y(index, y);

    PHYSICS_COMPONENTS[pcid].v = v;

    PHYSICS_COMPONENTS[pcid].alive = 1;

    SHOOTING_COMPONENTS[scid].bullets_n = PLAYER_BULLETS_INIT_N;
    SHOOTING_COMPONENTS[scid].fire_spacing = 128;
}

void player_manager_set_x(
    int   index,
    float value
) {
    int eid = PLAYER_ENTITY_INDEX[index];
    int pcid = ENTITY_PHYSICS_COMPONENT_INDEX[eid];
    int srid = PHYSICS_COMPONENT_SDL_RECT_INDEX[pcid];

    float x_min = 0 + PHYSICS_COMPONENTS[pcid].w / 2;
    float x_max = WINDOW_W - PHYSICS_COMPONENTS[pcid].w / 2;

    if (value < x_min) {
        value = x_min;
    } else if (value > x_max) {
        value = x_max;
    }

    PHYSICS_COMPONENTS[pcid].x = value;

    PHYSICS_COMPONENT_SDL_RECTS[srid].x = floor(PHYSICS_COMPONENTS[pcid].x - (PHYSICS_COMPONENTS[pcid].w / 2));
}

void player_manager_set_y(
    int   index,
    float value
) {
    int eid = PLAYER_ENTITY_INDEX[index];
    int pcid = ENTITY_PHYSICS_COMPONENT_INDEX[eid];
    int srid = PHYSICS_COMPONENT_SDL_RECT_INDEX[pcid];

    float y_min = 0 + PHYSICS_COMPONENTS[pcid].h / 2;
    float y_max = WINDOW_H - PHYSICS_COMPONENTS[pcid].h / 2;

    if (value < y_min) {
        value = y_min;
    } else if (value > y_max) {
        value = y_max;
    }

    PHYSICS_COMPONENTS[pcid].y = value;

    PHYSICS_COMPONENT_SDL_RECTS[srid].y = floor(PHYSICS_COMPONENTS[pcid].y - (PHYSICS_COMPONENTS[pcid].h / 2));
}

void player_manager_on_button_a_keydown(
    int index
) {
    int eid = PLAYER_ENTITY_INDEX[index];
    int scid = ENTITY_SHOOTING_COMPONENT_INDEX[eid];

    SHOOTING_COMPONENTS[scid].firing = 1;
    SHOOTING_COMPONENTS[scid].fire_time = SHOOTING_COMPONENTS[scid].fire_spacing * 1.0;
}

void player_manager_on_button_a_keyup(
    int index
) {
    int eid = PLAYER_ENTITY_INDEX[index];
    int scid = ENTITY_SHOOTING_COMPONENT_INDEX[eid];

    SHOOTING_COMPONENTS[scid].firing = 0;
    SHOOTING_COMPONENTS[scid].fire_time = 0.0;
}

void player_manager_fire(
    int index
) {
    int eid = PLAYER_ENTITY_INDEX[index];
    int scid = ENTITY_SHOOTING_COMPONENT_INDEX[eid];

    for (int i = 0; i < SHOOTING_COMPONENTS[scid].bullets_n; i++) { //ccabrera
        struct Bullet *bullet = bullet_manager_get_free(self->bullet_manager);

        if (bullet == NULL) {
            return;
        }

        bullet->alive = 1; //TODO: bullet_set_alive()

        bullet_set_x(bullet, self->x + self->w * PLAYER_BULLETS_OFFSET_X[i]);
        bullet_set_y(bullet, self->y + self->h * PLAYER_BULLETS_OFFSET_Y[i]);

        bullet->vx = PLAYER_BULLETS_VX;
        bullet->vy = PLAYER_BULLETS_VY;
    }
}

void player_manager_update(
    int index
) {
    self->vx = 0;
    self->vy = 0;

    if (self->keys->right) {
        self->vx = +1;
    }

    if (self->keys->left) {
        self->vx = -1;
    }

    if (self->keys->up) {
        self->vy = -1;
    }

    if (self->keys->down) {
        self->vy = +1;
    }

    player_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    player_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));

    if (self->keys->z) {
        self->fire_time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments
    }

    if (self->fire_time >= self->fire_spacing) {
        player_fire(self);
        self->fire_time = 0.0;
    }
}

void player_manager_render(
    int           index,
    SDL_Renderer  *renderer
) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
