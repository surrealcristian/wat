#ifndef HUD_H
#define HUD_H

#include "SDL.h"
#include "config.h"
#include "text.h"
#include "score.h"

#define HUD_SCORE_TEXT_BUFSIZE 32

struct HUD {
    struct Score *score;
    struct Text  *score_text;
};

void hud_init(
    struct HUD   *self,
    struct Score *score,
    struct Text  *score_text
);

void hud_update(
    struct HUD *self
);

void hud_render(
    struct HUD   *self,
    SDL_Renderer *renderer
);

#endif
