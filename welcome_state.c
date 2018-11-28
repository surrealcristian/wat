#include "welcome_state.h"

void welcome_state_init(
    struct WelcomeState *self,
    struct Text         *text
) {
    self->text = text;
}

void welcome_state_update(
    struct WelcomeState *self
) {

}

void welcome_state_render(
    struct WelcomeState *self,
    SDL_Renderer        *renderer
) {
    text_render(self->text, renderer);
}
