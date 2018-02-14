#include "SDL.h"
#include "background.h"
#include "sprite.h"
#include "game.h"

Game *game_new(void)
{
    Game *out = malloc(sizeof(Game));
    *out = (Game){};

    return out;
}

void game_free(Game *self)
{
    free(self);
}

void game_run(Game *game, SDL_Renderer *renderer)
{
    Background *background = background_new();
    Sprite *sprite1 = sprite_new();

    background_render(background, renderer);
    SDL_RenderPresent(renderer);

    sprite_set_width(sprite1, 32);
    sprite_set_height(sprite1, 32);
    sprite_set_velocity(sprite1, 7);
    sprite_set_angle(sprite1, 7);

    sprite_set_x(sprite1, 300);
    sprite_set_y(sprite1, 300);

    background_render(background, renderer);
    sprite_render(sprite1, renderer);
    SDL_RenderPresent(renderer);

    // -----

    SDL_Delay(1000);

    // -----

    sprite_set_x(sprite1, 200);
    sprite_set_y(sprite1, 200);

    background_render(background, renderer);
    sprite_render(sprite1, renderer);
    SDL_RenderPresent(renderer);

    // -----

    SDL_Delay(1000);

    // -----

    sprite_set_x(sprite1, 1000);
    sprite_set_y(sprite1, 1000);

    background_render(background, renderer);
    sprite_render(sprite1, renderer);
    SDL_RenderPresent(renderer);

    int key_left_pressed = 0;
    int key_right_pressed = 0;
    int key_up_pressed = 0;
    int key_down_pressed = 0;

    int keep_running = 1;

    while (keep_running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                if (!event.key.repeat) {
                    int sym = event.key.keysym.sym;

                    if (sym == SDLK_RIGHT) {
                        key_right_pressed = 1;
                    }

                    if (sym == SDLK_LEFT) {
                        key_left_pressed = 1;
                    }

                    if (sym == SDLK_UP) {
                        key_up_pressed = 1;
                    }

                    if (sym == SDLK_DOWN) {
                        key_down_pressed = 1;
                    }
                }
            } else if (event.type == SDL_KEYUP) {
                if (!event.key.repeat) {
                    int sym = event.key.keysym.sym;

                    if (sym == SDLK_RIGHT) {
                        key_right_pressed = 0;
                    }

                    if (sym == SDLK_LEFT) {
                        key_left_pressed = 0;
                    }

                    if (sym == SDLK_UP) {
                        key_up_pressed = 0;
                    }

                    if (sym == SDLK_DOWN) {
                        key_down_pressed = 0;
                    }
                }

            }
        }

        float prev_x;
        float prev_y;

        if (key_right_pressed) {
            prev_x = sprite_get_x(sprite1);
            sprite_set_x(sprite1, prev_x + 10);
        }

        if (key_left_pressed) {
            prev_x = sprite_get_x(sprite1);
            sprite_set_x(sprite1, prev_x - 10);
        }

        if (key_up_pressed) {
            prev_y = sprite_get_y(sprite1);
            sprite_set_y(sprite1, prev_y - 10);
        }

        if (key_down_pressed) {
            prev_y = sprite_get_y(sprite1);
            sprite_set_y(sprite1, prev_y + 10);
        }

        background_render(background, renderer);
        sprite_render(sprite1, renderer);
        SDL_RenderPresent(renderer);
    }

    sprite_free(sprite1);
    background_free(background);
}
