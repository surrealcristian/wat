#include "pause_state.h"

void pause_state_init(struct PauseState *self, struct Text *text) {
    self->text = text;

    self->pause_text = "PAUSE";
}

void pause_state_update(struct PauseState *self) {

}

void pause_state_render(struct PauseState *self, SDL_Renderer *renderer) {
    text_render_center(self->text, renderer, self->pause_text, floor(WINDOW_W / 2), floor(WINDOW_H / 2));
}
