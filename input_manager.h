#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "SDL.h"
#include "keys.h"
#include "game.h"
#include "player.h"

struct InputManager {
    struct Keys   *keys;
    struct Game   *game;
    struct Player *player;
};

void input_manager_init(
    struct InputManager *self,
    struct Keys           *keys,
    struct Game           *game,
    struct Player         *player
);

void input_manager_update(
    struct InputManager *self,
    SDL_Event             *event
);

#endif
