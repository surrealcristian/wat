#include "SDL.h"
#include "background.h"
#include "actor.h"
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
    Actor *actor1 = actor_new();

    background_render(background, renderer);
    SDL_RenderPresent(renderer);

    actor_set_width(actor1, 32);
    actor_set_height(actor1, 32);
    actor_set_velocity(actor1, 7);
    actor_set_angle(actor1, 7);

    actor_set_x(actor1, 300);
    actor_set_y(actor1, 300);

    background_render(background, renderer);
    actor_render(actor1, renderer);
    SDL_RenderPresent(renderer);

    // -----

    SDL_Delay(1000);

    // -----

    actor_set_x(actor1, 200);
    actor_set_y(actor1, 200);

    background_render(background, renderer);
    actor_render(actor1, renderer);
    SDL_RenderPresent(renderer);

    // -----

    SDL_Delay(1000);

    // -----

    actor_set_x(actor1, 1000);
    actor_set_y(actor1, 1000);

    background_render(background, renderer);
    actor_render(actor1, renderer);
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
            prev_x = actor_get_x(actor1);
            actor_set_x(actor1, prev_x + 10);
        }

        if (key_left_pressed) {
            prev_x = actor_get_x(actor1);
            actor_set_x(actor1, prev_x - 10);
        }

        if (key_up_pressed) {
            prev_y = actor_get_y(actor1);
            actor_set_y(actor1, prev_y - 10);
        }

        if (key_down_pressed) {
            prev_y = actor_get_y(actor1);
            actor_set_y(actor1, prev_y + 10);
        }

        background_render(background, renderer);
        actor_render(actor1, renderer);
        SDL_RenderPresent(renderer);
    }

    actor_free(actor1);
    background_free(background);
}
