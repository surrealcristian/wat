#include <stdio.h>
#include <math.h>
#include "SDL.h"

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480

#define ACTOR_N 3
#define ACTOR_RECT_N 1


// ============================================================================
// ACTOR INPUT COMPONENT
// ============================================================================
struct ActorInputComponent {
};
// ============================================================================
// END ACTOR INPUT COMPONENT
// ============================================================================


// ============================================================================
// ACTOR GRAPHICS COMPONENT
// ============================================================================
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

void actor_graphics_component_set_actor(struct ActorGraphicsComponent *self, struct Actor *actor)
{
    self->actor = actor;
}

void actor_graphics_component_set_position(struct ActorGraphicsComponent *self, float x, float y)
{
    self->x = x;
    self->y = y;

    //TODO: Update SDL_Rects
}

void actor_graphics_component_update(struct ActorGraphicsComponent *self, SDL_Renderer *renderer)
{
    for (int i = 0; i < ACTOR_RECT_N; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderFillRect(renderer, &self->rects[i]);
    }
}
// ============================================================================
// END ACTOR GRAPHICS COMPONENT
// ============================================================================


// ============================================================================
// ACTOR GRAPHICS COMPONENT MANAGER
// ============================================================================
struct ActorGraphicsComponentManager {
    struct ActorGraphicsComponent *actor_graphics_components;
};

void actor_graphics_component_manager_init(struct ActorGraphicsComponentManager *self, struct ActorGraphicsComponent *actor_graphics_components);
void actor_graphics_component_manager_update(struct ActorGraphicsComponentManager *self, SDL_Renderer *renderer);

void actor_graphics_component_manager_init(struct ActorGraphicsComponentManager *self, struct ActorGraphicsComponent *actor_graphics_components)
{
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

void actor_graphics_component_manager_update(struct ActorGraphicsComponentManager *self, SDL_Renderer *renderer)
{
    for (int i = 0; i < ACTOR_N; i++) {
        actor_graphics_component_update(&self->actor_graphics_components[i], renderer);
    }
}
// ============================================================================
// END ACTOR GRAPHICS COMPONENT MANAGER
// ============================================================================

// ============================================================================
// ACTOR
// ============================================================================
struct Actor {
    struct ActorGraphicsComponent *actor_graphics_component;
    float x;
    float y;
    int width;
    int height;
    int velocity;
    int angle;
};

void actor_set_actor_graphics_component(struct Actor *self, struct ActorGraphicsComponent *actor_graphics_component);
float actor_get_x(struct Actor *self);
void actor_set_x(struct Actor *self, float value);
float actor_get_y(struct Actor *self);
void actor_set_y(struct Actor *self, float value);
int actor_get_width(struct Actor *self);
void actor_set_width(struct Actor *self, int value);
int actor_get_height(struct Actor *self);
void actor_set_height(struct Actor *self, int value);
int actor_get_velocity(struct Actor *self);
void actor_set_velocity(struct Actor *self, int value);
int actor_get_angle(struct Actor *self);
void actor_set_angle(struct Actor *self, int value);
int actor_render(struct Actor *self, SDL_Renderer *renderer);


void actor_set_actor_graphics_component(struct Actor *self, struct ActorGraphicsComponent *actor_graphics_component)
{
    self->actor_graphics_component = actor_graphics_component;
}

float actor_get_x(struct Actor *self)
{
    return self->x;
}

void actor_set_x(struct Actor *self, float value)
{
    if (value < 0) {
        value = 0;
    } else if (value > WINDOW_WIDTH) {
        value = WINDOW_WIDTH;
    }

    self->x = value;

    /*
    SDL_Rect *p;

    for(
        p = (SDL_Rect*)utarray_front(self->rects);
        p != NULL;
        p = (SDL_Rect*)utarray_next(self->rects, p)
    ) {
        p->x = floor(self->x - (self->width / 2));
    }
    */
}

float actor_get_y(struct Actor *self)
{
    return self->y;
}

void actor_set_y(struct Actor *self, float value)
{
    if (value < 0) {
        value = 0;
    } else if (value > WINDOW_HEIGHT) {
        value = WINDOW_HEIGHT;
    }

    self->y = value;

    /*
    SDL_Rect *p;

    for(
        p = (SDL_Rect*)utarray_front(self->rects);
        p != NULL;
        p = (SDL_Rect*)utarray_next(self->rects, p)
    ) {
        p->y = floor(self->y - (self->width / 2));
    }
    */
}

int actor_get_width(struct Actor *self)
{
    return self->width;
}

void actor_set_width(struct Actor *self, int value)
{
    self->width = value;

    /*
    SDL_Rect *p;

    for(
        p = (SDL_Rect*)utarray_front(self->rects);
        p != NULL;
        p = (SDL_Rect*)utarray_next(self->rects, p)
    ) {
        p->w = value;
    }
    */
}

int actor_get_height(struct Actor *self)
{
    return self->height;
}

void actor_set_height(struct Actor *self, int value)
{
    self->height = value;

    /*
    SDL_Rect *p;

    for(
        p = (SDL_Rect*)utarray_front(self->rects);
        p != NULL;
        p = (SDL_Rect*)utarray_next(self->rects, p)
    ) {
        p->h = value;
    }
    */
}

int actor_get_velocity(struct Actor *self)
{
    return self->velocity;
}

void actor_set_velocity(struct Actor *self, int value)
{
    self->velocity = value;
}

int actor_get_angle(struct Actor *self)
{
    return self->angle;
}

void actor_set_angle(struct Actor *self, int value)
{
    self->angle = value;
}

int actor_render(struct Actor *self, SDL_Renderer *renderer)
{
    /*
    SDL_Rect *p;

    for(
        p = (SDL_Rect*)utarray_front(self->rects);
        p != NULL;
        p = (SDL_Rect*)utarray_next(self->rects, p)
    ) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        int tmp = SDL_RenderFillRect(renderer, p);

        if (tmp != 0) {
            return -1;
        }
    }
    */

    return 0;
}
// ============================================================================
// END ACTOR
// ============================================================================


struct ActorGraphicsComponent actor_graphics_component_array[ACTOR_N];
struct ActorGraphicsComponentManager actor_graphics_component_manager;


// ============================================================================
// GAME
// ============================================================================
struct Game {

};

struct Game *game_new(void);
void game_free(struct Game *self);
void game_run(struct Game *game, SDL_Renderer *renderer);

struct Game *game_new(void)
{
    struct Game *out = malloc(sizeof(struct Game));

    return out;
}

void game_free(struct Game *self)
{
    free(self);
}

void game_run(struct Game *self, SDL_Renderer *renderer)
{
    /*
    // actor1
    struct Actor *actor1 = actor_new();
    actor_set_width(actor1, 32);
    actor_set_height(actor1, 32);
    actor_set_velocity(actor1, 7);
    actor_set_angle(actor1, 7); 
    actor_set_x(actor1, 300);
    actor_set_y(actor1, 300);

    // actor2
    struct Actor *actor2 = actor_new();
    actor_set_width(actor2, 32);
    actor_set_height(actor2, 32);
    actor_set_velocity(actor2, 7);
    actor_set_angle(actor2, 7); 
    actor_set_x(actor2, 100);
    actor_set_y(actor2, 100);

    // actor3
    struct Actor *actor3 = actor_new();
    actor_set_width(actor3, 32);
    actor_set_height(actor3, 32);
    actor_set_velocity(actor3, 7);
    actor_set_angle(actor3, 7); 
    actor_set_x(actor3, 200);
    actor_set_y(actor3, 200);
    */

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
// ============================================================================
// END GAME
// ============================================================================


// ============================================================================
// MAIN
// ============================================================================
int main(void)
{
    // [init SDL]

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("ERROR: SDL_Init() (%s)", SDL_GetError());
        return 1;
    }

    SDL_Log("INFO: SDL_Init()");

    // [create window]

    SDL_Window *window = SDL_CreateWindow(
        "SHMUP",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if (window == NULL) {
        SDL_Log("ERROR: SDL_CreateWindow() (%s)", SDL_GetError());

        SDL_Quit();
        SDL_Log("INFO: SDL_Quit()");

        return 1;
    }

    SDL_Log("INFO: SDL_CreateWindow()");

    // [create renderer]

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

    // [create game]
    struct Game *game = game_new();
    game_run(game, renderer);

    // [destroy renderer]

    SDL_DestroyRenderer(renderer);
    SDL_Log("INFO: SDL_DestroyRenderer()");

    // [destroy window]

    SDL_DestroyWindow(window);
    SDL_Log("INFO: SDL_DestroyWindow()");

    // [quit SDL]

    SDL_Quit();
    SDL_Log("INFO: SDL_Quit()");

    return 0;
}
// ============================================================================
// END MAIN
// ============================================================================
