#include "enemy.h"

void enemy_init(struct Enemy *self, float x, float y, int w, int h, int v) {
    self->w = w;
    self->rect.w = self->w;

    self->h = h;
    self->rect.h = self->h;

    enemy_set_x(self, x);
    enemy_set_y(self, y);

    self->v = v;

    self->alive = 0;
}

void enemy_set_x(struct Enemy *self, float value) {
    float x_min = 0 - self->w + 1;
    float x_max = WINDOW_W + self->w + 1;

    if (value < x_min || value > x_max) {
        self->alive = 0;
        return;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

void enemy_set_y(struct Enemy *self, float value) {
    float y_min = 0 - self->h - 1;
    float y_max = WINDOW_H + self->h + 1;

    if (value < y_min || value > y_max) {
        self->alive = 0;
        return;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

void enemy_update(struct Enemy *self) {
    enemy_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    enemy_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));
}

void enemy_render(struct Enemy *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
