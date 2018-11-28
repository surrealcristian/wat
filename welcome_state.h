#ifndef WELCOME_STATE_H
#define WELCOME_STATE_H

#include "SDL.h"
#include "config.h"
#include "text.h"

struct WelcomeState {
    struct Text *text;
};

void welcome_state_init(
    struct WelcomeState *self,
    struct Text         *text
);

void welcome_state_update(
    struct WelcomeState *self
);

void welcome_state_render(
    struct WelcomeState *self,
    SDL_Renderer        *renderer
);

#endif
