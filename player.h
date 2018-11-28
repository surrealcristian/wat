#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "keys.h"
#include "bullet_manager.h"

struct Player {
    struct Keys          *keys;
    struct BulletManager *bullet_manager;
    float                x;
    float                y;
    int                  w;
    int                  h;
    int                  v;
    int                  vx;
    int                  vy;
    int                  alive;
    int                  bullets_n;
    int                  firing;
    int                  fire_spacing;
    float                fire_time;
    SDL_Rect             rect;
};

void player_init(
    struct Player        *self,
    struct Keys          *keys,
    struct BulletManager *bullet_manager,
    float                x,
    float                y,
    int                  w,
    int                  h,
    int                  v
);

void player_set_x(
    struct Player *self,
    float         value
);

void player_set_y(
    struct Player *self,
    float         value
);

void player_on_button_a_keydown(
    struct Player *self
);

void player_on_button_a_keyup(
    struct Player *self
);

void player_fire(
    struct Player *self
);

void player_update(
    struct Player *self
);

void player_render(
    struct Player *self,
    SDL_Renderer  *renderer
);

#endif
