#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "welcome_state.h"
#include "in_game_state.h"
#include "pause_state.h"
#include "input_manager.h"

struct Game {
    struct InputManager *input_manager;
    struct WelcomeState   *welcome_state;
    struct InGameState    *in_game_state;
    struct PauseState     *pause_state;
    int                   state;
};

void game_init(
    struct Game         *self,
    struct WelcomeState *welcome_state,
    struct InGameState  *in_game_state,
    struct PauseState   *pause_state
);

void game_set_input_manager(
    struct Game           *self,
    struct InputManager *input_manager
);

void game_run(
    struct Game  *self,
    SDL_Renderer *renderer
);

#endif
