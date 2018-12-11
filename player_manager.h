#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "keys.h"
#include "bullet_manager.h"

void player_manager_init(
    int    index,
    float  x,
    float  y,
    int    w,
    int    h,
    int    v
);

void player_manager_set_x(
    int   index,
    float value
);

void player_manager_set_y(
    int   index,
    float value
);

void player_manager_on_button_a_keydown(
    int index
);

void player_manager_on_button_a_keyup(
    int index
);

void player_manager_fire(
    int index
);

void player_manager_update(
    int index
);

#endif
