#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "in_game_state.h"
#include "pause_state.h"
#include "input_component.h"

struct Game {
    struct InputComponent *input_component;
    struct InGameState *in_game_state;
    int state;
};

void game_init(struct Game *self, struct InGameState *in_game_state);
void game_set_input_component(struct Game *self, struct InputComponent *input_component);
void game_run(struct Game *self, SDL_Renderer *renderer);

#endif
