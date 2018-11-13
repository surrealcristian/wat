#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "SDL.h"
#include "keys.h"
#include "game.h"
#include "player.h"

struct InputComponent {
    struct Keys *keys;
    struct Game *game;
    struct Player *player;
};

void input_component_init(
    struct InputComponent *self,
    struct Keys *keys,
    struct Game *game,
    struct Player *player
);
void input_component_update(struct InputComponent *self, SDL_Event *event);

#endif
