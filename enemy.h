#ifndef ENEMY_H
#define ENEMY_H

#include "config.h"
#include "SDL.h"

struct Enemy {
    float    x;
    float    y;
    int      w;
    int      h;
    int      v;
    int      vx;
    int      vy;
    int      alive;
    SDL_Rect rect;
};

void enemy_init(
    struct Enemy *self,
    float         x,
    float         y,
    int           w,
    int           h,
    int           v
);

void enemy_set_x(
    struct Enemy *self,
    float        value
);

void enemy_set_y(
    struct Enemy *self,
    float        value
);


void enemy_update(
    struct Enemy *self
);

void enemy_render(
    struct Enemy *self,
    SDL_Renderer *renderer
);

#endif
