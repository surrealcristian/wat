#include "pause_state.h"

void pause_state_init(
    struct PauseState *self,
    struct Text       *text
) {
    self->text = text;
}

void pause_state_update(
    struct PauseState *self
) {

}

void pause_state_render(
    struct PauseState *self,
    SDL_Renderer      *renderer
) {
    text_render(self->text, renderer);
}
