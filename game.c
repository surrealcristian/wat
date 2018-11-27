#include "game.h"

void game_init(struct Game *self, struct InGameState *in_game_state, struct PauseState *pause_state) {
    self->in_game_state = in_game_state;
    self->pause_state = pause_state;

    self->state = STATE_IN_GAME;
}

void game_set_input_component(struct Game *self, struct InputComponent *input_component) {
    self->input_component = input_component;
}

void game_run(struct Game *self, SDL_Renderer *renderer) {
    int keep_running = 1;

    Uint64 previous = SDL_GetPerformanceCounter();
    double lag = 0.0;

    while (keep_running) {
        Uint64 current = SDL_GetPerformanceCounter();
        double elapsed = performance_counters_to_ms(previous, current);

        //SDL_Log("INFO: Frame time: %f ms.", elapsed);

        previous = current;
        lag += elapsed;

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_running = 0;
            }

            input_component_update(self->input_component, &event);
        }

        while (lag >= MS_PER_UPDATE) {
            if (self->state == STATE_IN_GAME) {
                in_game_state_update(self->in_game_state);
            } else if (self->state == STATE_PAUSE) {
                // pause_state_update(self->pause_state);
            }

            lag -= MS_PER_UPDATE;
        }

        // RENDER

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // grey
        SDL_RenderClear(renderer);

        if (self->state == STATE_IN_GAME) {
            in_game_state_render(self->in_game_state, renderer);
        } else if (self->state == STATE_PAUSE) {
            pause_state_render(self->pause_state, renderer);
        }

        SDL_RenderPresent(renderer);

        //SDL_Delay(SLEEP_MS);
    }
}
