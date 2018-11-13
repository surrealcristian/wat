#ifndef BULLET_H
#define BULLET_H

#include "config.h"
#include "SDL.h"

struct Bullet {
    float x;
    float y;
    int w;
    int h;
    int v;
    int vx;
    int vy;
    int alive;
    SDL_Rect rect;
};

void bullet_init(struct Bullet *self, float x, float y, int w, int h, int v);
float bullet_get_x(struct Bullet *self);
void bullet_set_x(struct Bullet *self, float value);
float bullet_get_y(struct Bullet *self);
void bullet_set_y(struct Bullet *self, float value);
void bullet_update(struct Bullet *self);
void bullet_render(struct Bullet *self, SDL_Renderer *renderer);

#endif
