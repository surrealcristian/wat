#include <stdio.h>
#include <math.h>
#include "SDL.h"

#define WINDOW_W 480
#define WINDOW_H 480

#define ACTOR_N 3
#define ACTOR_RECT_N 1


// NAV_ActorInputComponent
struct ActorInputComponent {
};
// END NAV_ActorInputComponent


// NAV_ActorGraphicsComponent
struct ActorGraphicsComponent {
    struct Actor *actor;
    float x;
    float y;
    float w;
    float h;
    SDL_Rect rects[ACTOR_RECT_N];
};

void actor_graphics_component_set_actor(struct ActorGraphicsComponent *self, struct Actor *actor);
void actor_graphics_component_set_position(struct ActorGraphicsComponent *self, float x, float y);
void actor_graphics_component_update(struct ActorGraphicsComponent *self, SDL_Renderer *renderer);

void actor_graphics_component_set_actor(struct ActorGraphicsComponent *self, struct Actor *actor) {
    self->actor = actor;
}

void actor_graphics_component_set_position(struct ActorGraphicsComponent *self, float x, float y) {
    self->x = x;
    self->y = y;

    //TODO: Update SDL_Rects
}

void actor_graphics_component_update(struct ActorGraphicsComponent *self, SDL_Renderer *renderer) {
    for (int i = 0; i < ACTOR_RECT_N; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderFillRect(renderer, &self->rects[i]);
    }
}
// END NAV_ActorGraphicsComponent


// NAV_ActorGraphicsComponentManager
struct ActorGraphicsComponentManager {
    struct ActorGraphicsComponent *actor_graphics_components;
};

void actor_graphics_component_manager_init(struct ActorGraphicsComponentManager *self, struct ActorGraphicsComponent *actor_graphics_components);
void actor_graphics_component_manager_update(struct ActorGraphicsComponentManager *self, SDL_Renderer *renderer);

void actor_graphics_component_manager_init(struct ActorGraphicsComponentManager *self, struct ActorGraphicsComponent *actor_graphics_components) {
    self->actor_graphics_components = actor_graphics_components;

    for (int i = 0; i < ACTOR_N; i++) {
        struct ActorGraphicsComponent *c = &self->actor_graphics_components[i];

        c->x = 0;
        c->y = 0;
        c->w = 20;
        c->h = 40;

        for (int j = 0; j < ACTOR_RECT_N; j++) {
            SDL_Rect *r = &c->rects[j];

            r->x = floor(c->x - (c->w / 2));
            r->y = floor(c->y - (c->h / 2));
            r->w = c->w;
            r->h = c->h;
        }
    }
}

void actor_graphics_component_manager_update(struct ActorGraphicsComponentManager *self, SDL_Renderer *renderer) {
    for (int i = 0; i < ACTOR_N; i++) {
        actor_graphics_component_update(&self->actor_graphics_components[i], renderer);
    }
}
// END NAV_ActorGraphicsComponentManager


// NAV_Actor
struct Actor {
    struct ActorGraphicsComponent *actor_graphics_component;
    float x;
    float y;
    int w;
    int h;
    int velocity;
    int angle;
};

void actor_set_actor_graphics_component(struct Actor *self, struct ActorGraphicsComponent *actor_graphics_component);
float actor_get_x(struct Actor *self);
void actor_set_x(struct Actor *self, float value);
float actor_get_y(struct Actor *self);
void actor_set_y(struct Actor *self, float value);
int actor_get_w(struct Actor *self);
void actor_set_w(struct Actor *self, int value);
int actor_get_h(struct Actor *self);
void actor_set_h(struct Actor *self, int value);
int actor_get_velocity(struct Actor *self);
void actor_set_velocity(struct Actor *self, int value);
int actor_get_angle(struct Actor *self);
void actor_set_angle(struct Actor *self, int value);


void actor_set_actor_graphics_component(struct Actor *self, struct ActorGraphicsComponent *actor_graphics_component) {
    self->actor_graphics_component = actor_graphics_component;
}

float actor_get_x(struct Actor *self) {
    return self->x;
}

void actor_set_x(struct Actor *self, float value) {
    if (value < 0) {
        value = 0;
    } else if (value > WINDOW_W) {
        value = WINDOW_W;
    }

    self->x = value;

    actor_graphics_component_set_position(self->actor_graphics_component, self->x, self->y);
}

float actor_get_y(struct Actor *self) {
    return self->y;
}

void actor_set_y(struct Actor *self, float value) {
    if (value < 0) {
        value = 0;
    } else if (value > WINDOW_H) {
        value = WINDOW_H;
    }

    self->y = value;

    actor_graphics_component_set_position(self->actor_graphics_component, self->x, self->y);
}

int actor_get_w(struct Actor *self) {
    return self->w;
}

void actor_set_w(struct Actor *self, int value) {
    self->w = value;
}

int actor_get_h(struct Actor *self) {
    return self->h;
}

void actor_set_h(struct Actor *self, int value) {
    self->h = value;
}

int actor_get_velocity(struct Actor *self) {
    return self->velocity;
}

void actor_set_velocity(struct Actor *self, int value) {
    self->velocity = value;
}

int actor_get_angle(struct Actor *self) {
    return self->angle;
}

void actor_set_angle(struct Actor *self, int value) {
    self->angle = value;
}
// END NAV_Actor


// NAV_ActorManager
struct ActorManager {
    struct Actor *actors;
};

void actor_manager_init(struct ActorManager *self, struct Actor *actors, struct ActorGraphicsComponent *actor_graphics_components);

void actor_manager_init(struct ActorManager *self, struct Actor *actors, struct ActorGraphicsComponent *actor_graphics_components) {
    self->actors = actors;

    for (int i = 0; i < ACTOR_N; i++) {
        struct Actor *a = &self->actors[i];

        a->actor_graphics_component = &actor_graphics_components[i];

        a->x = 0;
        a->y = 0;
        a->w = 20;
        a->h = 40;
        a->velocity = 0;
        a->angle = 0;
    }
}
// END NAV_ActorManager


struct ActorGraphicsComponent actor_graphics_component_array[ACTOR_N];
struct ActorGraphicsComponentManager actor_graphics_component_manager;
struct Actor actor_array[ACTOR_N];
struct ActorManager actor_manager;
struct Game game;


// NAV_Game
struct Game {

};

void game_run(struct Game *game, SDL_Renderer *renderer);

void game_run(struct Game *self, SDL_Renderer *renderer) {
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
                /*
                prev_x = actor_get_x(actor1);
                actor_set_x(actor1, prev_x + distance);
                */
            }

            if (key_left_pressed) {
                /*
                prev_x = actor_get_x(actor1);
                actor_set_x(actor1, prev_x - distance);
                */
            }

            if (key_up_pressed) {
                /*
                prev_y = actor_get_y(actor1);
                actor_set_y(actor1, prev_y - distance);
                */
            }

            if (key_down_pressed) {
                /*
                prev_y = actor_get_y(actor1);
                actor_set_y(actor1, prev_y + distance);
                */
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

            actor_graphics_component_manager_update(&actor_graphics_component_manager, renderer);

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
}
// END NAV_Game


// NAV_Main
int main(void) {
    // init SDL

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("ERROR: SDL_Init() (%s)", SDL_GetError());
        return 1;
    }

    SDL_Log("INFO: SDL_Init()");

    // create window

    SDL_Window *window = SDL_CreateWindow(
        "SHMUP",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_W,
        WINDOW_H,
        0
    );

    if (window == NULL) {
        SDL_Log("ERROR: SDL_CreateWindow() (%s)", SDL_GetError());

        SDL_Quit();
        SDL_Log("INFO: SDL_Quit()");

        return 1;
    }

    SDL_Log("INFO: SDL_CreateWindow()");

    // create renderer

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        //SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        SDL_RENDERER_ACCELERATED
        //SDL_RENDERER_SOFTWARE
    );

    if (renderer == NULL) {
        SDL_Log("ERROR: SDL_CreateRenderer() (%s)", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Log("INFO: SDL_DestroyWindow()");

        SDL_Quit();
        SDL_Log("INFO: SDL_Quit()");

        return 1;
    }

    SDL_Log("INFO: SDL_CreateRenderer()");

    actor_graphics_component_manager_init(&actor_graphics_component_manager, actor_graphics_component_array);
    actor_manager_init(&actor_manager, actor_array, actor_graphics_component_array);

    game_run(&game, renderer);

    // destroy renderer

    SDL_DestroyRenderer(renderer);
    SDL_Log("INFO: SDL_DestroyRenderer()");

    // destroy window

    SDL_DestroyWindow(window);
    SDL_Log("INFO: SDL_DestroyWindow()");

    // quit SDL

    SDL_Quit();
    SDL_Log("INFO: SDL_Quit()");

    return 0;
}
// END NAV_Main
