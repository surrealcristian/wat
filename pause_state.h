#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include "SDL.h"
#include "config.h"
#include "text.h"

struct PauseState {
    struct Text *text;
};

void pause_state_init(
    struct PauseState *self,
    struct Text       *text
);

void pause_state_update(
    struct PauseState *self
);

void pause_state_render(
    struct PauseState *self,
    SDL_Renderer      *renderer
);

#endif
