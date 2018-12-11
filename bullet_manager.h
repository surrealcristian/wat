#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "SDL.h"

//TODO: bullet.h start

void bullet_manager_init(
    int   index,
    float x,
    float y,
    int   w,
    int   h,
    int   v
);

float bullet_manager_get_x(
    int index
);

void bullet_manager_set_x(
    int   index,
    float value
);

float bullet_manager_get_y(
    int index
);

void bullet_manager_set_y(
    int   index,
    float value
);

void bullet_manager_update(
    int index
);

void bullet_manager_render(
    int          index,
    SDL_Renderer *renderer
);

//TODO: bullet.h end


void bullet_manager_init_all(
    int                  w,
    int                  h,
    int                  v
);

int bullet_manager_get_free();

void bullet_manager_update_all();

void bullet_manager_render_all(
    SDL_Renderer *renderer
);

#endif
