#include "input_manager.h"

void input_manager_init(
    struct InputManager *self,
    struct Game           *game
) {
    self->game   = game;
}

void input_manager_update(
    struct InputManager *self,
    SDL_Event             *event
) {
    int sym;

    if (event->type == SDL_KEYDOWN) {
        if (event->key.repeat) {
            return;
        }

        sym = event->key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            KEYS.right = 1;
        } else if (sym == SDLK_LEFT) {
            KEYS.left = 1;
        } else if (sym == SDLK_UP) {
            KEYS.up = 1;
        } else if (sym == SDLK_DOWN) {
            KEYS.down = 1;
        } else if (sym == SDLK_z) {
            KEYS.z = 1;

            if (self->game->state == STATE_IN_GAME) {
                player_manager_on_button_a_keydown(0);
            }
        }
    } else if (event->type == SDL_KEYUP) {
        if (event->key.repeat) {
            return;
        }

        sym = event->key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            KEYS.right = 0;
        } else if (sym == SDLK_LEFT) {
            KEYS.left = 0;
        } else if (sym == SDLK_UP) {
            KEYS.up = 0;
        } else if (sym == SDLK_DOWN) {
            KEYS.down = 0;
        } else if (sym == SDLK_z) {
            KEYS.z = 0;

            if (self->game->state == STATE_WELCOME) {
                self->game->state = STATE_IN_GAME;
            } else if (self->game->state == STATE_IN_GAME) {
                player_manager_on_button_a_keyup(0);
            }
        } else if (sym == SDLK_ESCAPE) {
            if (self->game->state == STATE_IN_GAME) {
                self->game->state = STATE_PAUSE;
            } else if (self->game->state == STATE_PAUSE) {
                self->game->state = STATE_IN_GAME;
            }
        }
    }
}
