#include "hud.h"

char HUD_SCORE_TEXT[HUD_SCORE_TEXT_BUFSIZE];

void hud_init(
    struct HUD   *self,
    struct Score *score,
    struct Text  *score_text
) {
    self->score      = score;
    self->score_text = score_text;
}

void hud_update(
    struct HUD *self
) {
    snprintf(HUD_SCORE_TEXT, HUD_SCORE_TEXT_BUFSIZE, "%lu", self->score->value);

    text_set_value(self->score_text, HUD_SCORE_TEXT);
}

void hud_render(
    struct HUD   *self,
    SDL_Renderer *renderer
) {
    text_render(self->score_text, renderer);
}
