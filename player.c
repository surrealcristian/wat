#include "player.h"

void player_init(
    struct Player        *self,
    struct Keys          *keys,
    struct BulletManager *bullet_manager,
    float                x,
    float                y,
    int                  w,
    int                  h,
    int                  v
) {
    self->keys           = keys;
    self->bullet_manager = bullet_manager;

    self->w = w;
    self->h = h;

    self->rect.w = self->w;
    self->rect.h = self->h;

    player_set_x(self, x);
    player_set_y(self, y);

    self->v = v;

    self->alive = 1;

    self->bullets_n = PLAYER_BULLETS_INIT_N;

    self->fire_spacing = 128;
}

void player_set_x(
    struct Player *self,
    float         value
) {
    float x_min = 0 + self->w / 2;
    float x_max = WINDOW_W - self->w / 2;

    if (value < x_min) {
        value = x_min;
    } else if (value > x_max) {
        value = x_max;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

void player_set_y(
    struct Player *self,
    float         value
) {
    float y_min = 0 + self->h / 2;
    float y_max = WINDOW_H - self->h / 2;

    if (value < y_min) {
        value = y_min;
    } else if (value > y_max) {
        value = y_max;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

void player_on_button_a_keydown(
    struct Player *self
) {
    self->firing = 1;
    self->fire_time = self->fire_spacing * 1.0;
}

void player_on_button_a_keyup(
    struct Player *self
) {
    self->firing = 0;
    self->fire_time = 0.0;
}

void player_fire(
    struct Player *self
) {
    for (int i = 0; i < self->bullets_n; i++) {
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

void player_update(
    struct Player *self
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

void player_render(
    struct Player *self,
    SDL_Renderer  *renderer
) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
