#include "game.h"
#include "utlist.h"

Game *game_new(void)
{
    Game *out = malloc(sizeof(Game));

    return out;
}

void game_free(Game *self)
{
    free(self);
}

void game_run(Game *self, SDL_Renderer *renderer)
{
    // actor1
    Actor *actor1 = actor_new();
    actor_add_rect(actor1);
    actor_set_width(actor1, 32);
    actor_set_height(actor1, 32);
    actor_set_velocity(actor1, 7);
    actor_set_angle(actor1, 7); 
    actor_set_x(actor1, 300);
    actor_set_y(actor1, 300);

    // actor2
    Actor *actor2 = actor_new();
    actor_add_rect(actor2);
    actor_set_width(actor2, 32);
    actor_set_height(actor2, 32);
    actor_set_velocity(actor2, 7);
    actor_set_angle(actor2, 7); 
    actor_set_x(actor2, 100);
    actor_set_y(actor2, 100);

    // actor3
    Actor *actor3 = actor_new();
    actor_add_rect(actor3);
    actor_set_width(actor3, 32);
    actor_set_height(actor3, 32);
    actor_set_velocity(actor3, 7);
    actor_set_angle(actor3, 7); 
    actor_set_x(actor3, 200);
    actor_set_y(actor3, 200);

    Actor *actors = NULL;
    LL_APPEND(actors, actor1);
    LL_APPEND(actors, actor2);
    LL_APPEND(actors, actor3);

    int key_left_pressed = 0;
    int key_right_pressed = 0;
    int key_up_pressed = 0;
    int key_down_pressed = 0;

    int keep_running = 1;

    double loop_tick_sleep_ms = 2;

    Uint64 loop_tick_start = 0;
    Uint64 loop_tick_end_no_delay = 0;
    Uint64 loop_tick_end = 0;
    double loop_tick_no_delay_ms = 0;
    double loop_tick_ms;

    double update_ideal_ms = 8.33;
    double update_acum_ms = 0;
    Uint64 update_tick_start = 0;
    Uint64 update_tick_end = 0;
    double update_tick_ms = 0;

    double render_ideal_ms = 16.66;
    double render_acum_ms = 0;
    Uint64 render_tick_start = 0;
    Uint64 render_tick_end = 0;
    double render_tick_ms = 0;

    int sym;

    float prev_x;
    float prev_y;

    update_tick_start = SDL_GetPerformanceCounter();
    render_tick_start = SDL_GetPerformanceCounter();

    while (keep_running) {
        loop_tick_start = SDL_GetPerformanceCounter();

        update_tick_end = SDL_GetPerformanceCounter();

        update_tick_ms = (double)((update_tick_end - update_tick_start) * 1000) / SDL_GetPerformanceFrequency();

        update_acum_ms = update_acum_ms + update_tick_ms;

        if (update_acum_ms >= update_ideal_ms) {
            SDL_Event event;

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    keep_running = 0;
                } else if (event.type == SDL_KEYDOWN) {
                    if (!event.key.repeat) {
                        sym = event.key.keysym.sym;

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
                        sym = event.key.keysym.sym;

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

            double actor_px_per_second = 250;
            double distance = (actor_px_per_second * update_acum_ms) / 1000;

            if (key_right_pressed) {
                prev_x = actor_get_x(actor1);
                actor_set_x(actor1, prev_x + distance);
            }

            if (key_left_pressed) {
                prev_x = actor_get_x(actor1);
                actor_set_x(actor1, prev_x - distance);
            }

            if (key_up_pressed) {
                prev_y = actor_get_y(actor1);
                actor_set_y(actor1, prev_y - distance);
            }

            if (key_down_pressed) {
                prev_y = actor_get_y(actor1);
                actor_set_y(actor1, prev_y + distance);
            }

            update_acum_ms = 0;
        }

        update_tick_start = SDL_GetPerformanceCounter();

        // --------------------------------------------------------------------
        // Render
        // --------------------------------------------------------------------

        render_tick_end = SDL_GetPerformanceCounter();

        render_tick_ms = (double)((render_tick_end - render_tick_start) * 1000) / SDL_GetPerformanceFrequency();

        render_acum_ms = render_acum_ms + render_tick_ms;

        if (render_acum_ms >= render_ideal_ms) {
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE); // grey
            int tmp = SDL_RenderClear(renderer);

            if (tmp != 0) {
                SDL_Log("ERROR: SDL_RenderClear() (%s)", SDL_GetError());
            }

            Actor *actor_elem;

            LL_FOREACH(actors, actor_elem) {
                actor_render(actor_elem, renderer);
            }

            actor_render(actor1, renderer);

            SDL_RenderPresent(renderer);

            render_acum_ms = 0;
        }

        render_tick_start = SDL_GetPerformanceCounter();

        loop_tick_end_no_delay = SDL_GetPerformanceCounter();

        loop_tick_no_delay_ms = (double)((loop_tick_end_no_delay - loop_tick_start) * 1000) / SDL_GetPerformanceFrequency();

        if (loop_tick_sleep_ms > loop_tick_no_delay_ms) {
            SDL_Delay(loop_tick_sleep_ms - loop_tick_no_delay_ms);
        }

        loop_tick_end = SDL_GetPerformanceCounter();

        loop_tick_ms = (double)((loop_tick_end - loop_tick_start) * 1000) / SDL_GetPerformanceFrequency();
    }

    actor_free(actor1);
}
