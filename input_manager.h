#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "SDL.h"
#include "keys.h"
#include "game.h"

struct InputManager {
    struct Game   *game;
};

void input_manager_init(
    struct InputManager *self,
    struct Game           *game
);

void input_manager_update(
    struct InputManager *self,
    SDL_Event             *event
);

#endif
