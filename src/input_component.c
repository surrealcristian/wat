#include "input_component.h"

void input_component_init(
    struct InputComponent *self,
    struct Keys *keys,
    struct Game *game,
    struct Player *player
) {
    self->keys = keys;
    self->game = game;
    self->player = player;
}

void input_component_update(struct InputComponent *self, SDL_Event *event) {
    int sym;

    if (event->type == SDL_KEYDOWN) {
        if (event->key.repeat) {
            return;
        }

        sym = event->key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            self->keys->right = 1;
        } else if (sym == SDLK_LEFT) {
            self->keys->left = 1;
        } else if (sym == SDLK_UP) {
            self->keys->up = 1;
        } else if (sym == SDLK_DOWN) {
            self->keys->down = 1;
        } else if (sym == SDLK_z) {
            self->keys->z = 1;

            player_on_button_a_keydown(self->player);
        }
    } else if (event->type == SDL_KEYUP) {
        if (event->key.repeat) {
            return;
        }

        sym = event->key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            self->keys->right = 0;
        } else if (sym == SDLK_LEFT) {
            self->keys->left = 0;
        } else if (sym == SDLK_UP) {
            self->keys->up = 0;
        } else if (sym == SDLK_DOWN) {
            self->keys->down = 0;
        } else if (sym == SDLK_z) {
            self->keys->z = 0;

            player_on_button_a_keyup(self->player);
        } else if (sym == SDLK_ESCAPE) {
            if (self->game->state == STATE_IN_GAME) {
                self->game->state = STATE_PAUSE;
            } else if (self->game->state == STATE_PAUSE) {
                self->game->state = STATE_IN_GAME;
            }
        }
    }
}
