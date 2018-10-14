#include "bullet.h"

void bullet_init(struct Bullet *self, float x, float y, int w, int h, int v) {
    self->w = w;
    self->rect.w = self->w;

    self->h = h;
    self->rect.h = self->h;

    bullet_set_x(self, x);
    bullet_set_y(self, y);

    self->v = v;

    self->alive = 0;
}

float bullet_get_x(struct Bullet *self) {
    return self->x;
}

void bullet_set_x(struct Bullet *self, float value) {
    float x_min = 0 - self->w - 1;
    float x_max = WINDOW_W + self->w + 1;

    if (value < x_min || value > x_max) {
        self->alive = 0;
        return;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

float bullet_get_y(struct Bullet *self) {
    return self->y;
}

void bullet_set_y(struct Bullet *self, float value) {
    float y_min = 0 - self->h - 1;
    float y_max = WINDOW_H + self->h + 1;

    if (value < y_min || value > y_max) {
        self->alive = 0;
        return;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

void bullet_update(struct Bullet *self) {
    bullet_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    bullet_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));
}

void bullet_render(struct Bullet *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
