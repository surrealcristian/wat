#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include "SDL.h"

struct PauseState {
};

void pause_state_update(struct PauseState *pause_state);
void pause_state_render(struct PauseState *pause_state, SDL_Renderer *renderer);

#endif
