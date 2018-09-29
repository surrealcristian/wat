#include <stdio.h>
#include <math.h>
#include "SDL.h"

#define WINDOW_W 480
#define WINDOW_H 480

#define ACTOR_N 3
#define ACTOR_RECT_N 1


// NAV_Declarations
//   NAV_Keys
//   NAV_InputComponent


// NAV_Declarations

// NAV_Keys
struct Keys {
    int left_pressed;
    int right_pressed;
    int up_pressed;
    int down_pressed;
};
// END NAV_Keys


// NAV_InputComponent
struct InputComponent {
    struct Keys *keys;
};

void input_component_set_keys(struct InputComponent *self, struct Keys *keys);
void input_component_update(struct InputComponent *self, SDL_Event *event);
// END NAV_InputComponent


// NAV_Actor
struct Actor {
    float x;
    float y;
    int w;
    int h;
    int velocity;
};

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
// END NAV_Actor


// NAV_Player
struct Player {
    float x;
    float y;
    int w;
    int h;
    int velocity;
    SDL_Rect rect;
};

void player_init(struct Player *self, float x, float y, int w, int h, int velocity);
float player_get_x(struct Player *self);
void player_set_x(struct Player *self, float value);
float player_get_y(struct Player *self);
void player_set_y(struct Player *self, float value);
int player_get_w(struct Player *self);
void player_set_w(struct Player *self, int value);
int player_get_h(struct Player *self);
void player_set_h(struct Player *self, int value);
int player_get_velocity(struct Player *self);
void player_set_velocity(struct Player *self, int value);
void player_update(struct Player *self);
void player_render(struct Player *self, SDL_Renderer *renderer);
// END NAV_Player


// NAV_ActorManager
struct ActorManager {
    struct Actor *actors;
};

void actor_manager_init(struct ActorManager *self, struct Actor *actors);
// END NAV_ActorManager


// NAV_MiscFunctions
double performance_counters_to_ms(Uint64 start, Uint64 end);
// END NAV_MiscFunctions


// NAV_Game
struct Game {

};

void game_run(struct Game *self, SDL_Renderer *renderer);
// END NAV_Game


// NAV_Main
int main(void);
// END NAV_Main

// END NAV_Declarations


// NAV_GlobalVariables
struct Keys keys;
struct InputComponent input_component;
struct Actor actor_array[ACTOR_N];
struct ActorManager actor_manager;
struct Player player;
struct Game game;
// END NAV_GlobalVariables


// NAV_Code

// NAV_InputComponent
void input_component_set_keys(struct InputComponent *self, struct Keys *keys) {
    self->keys = keys;
}

void input_component_update(struct InputComponent *self, SDL_Event *event) {
    int sym;

    if (event->type == SDL_KEYDOWN) {
        if (event->key.repeat) {
            return;
        }

        sym = event->key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            self->keys->right_pressed = 1;
        } else if (sym == SDLK_LEFT) {
            self->keys->left_pressed = 1;
        } else if (sym == SDLK_UP) {
            self->keys->up_pressed = 1;
        } else if (sym == SDLK_DOWN) {
            self->keys->down_pressed = 1;
        }
    } else if (event->type == SDL_KEYUP) {
        if (event->key.repeat) {
            return;
        }

        sym = event->key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            self->keys->right_pressed = 0;
        } else if (sym == SDLK_LEFT) {
            self->keys->left_pressed = 0;
        } else if (sym == SDLK_UP) {
            self->keys->up_pressed = 0;
        } else if (sym == SDLK_DOWN) {
            self->keys->down_pressed = 0;
        }
    }
}
// END NAV_InputComponent


// NAV_Actor
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
// END NAV_Actor


// NAV_Player
void player_init(struct Player *self, float x, float y, int w, int h, int velocity) {
    player_set_w(self, w);
    player_set_h(self, h);
    player_set_x(self, x);
    player_set_y(self, y);
    player_set_velocity(self, velocity);
}

float player_get_x(struct Player *self) {
    return self->x;
}

void player_set_x(struct Player *self, float value) {
    if (value < 0) {
        value = 0;
    } else if (value > WINDOW_W) {
        value = WINDOW_W;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

float player_get_y(struct Player *self) {
    return self->y;
}

void player_set_y(struct Player *self, float value) {
    if (value < 0) {
        value = 0;
    } else if (value > WINDOW_H) {
        value = WINDOW_H;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

int player_get_w(struct Player *self) {
    return self->w;
}

void player_set_w(struct Player *self, int value) {
    self->w = value;
    self->rect.w = self->w;
}

int player_get_h(struct Player *self) {
    return self->h;
}

void player_set_h(struct Player *self, int value) {
    self->h = value;
    self->rect.h = self->h;
}

int player_get_velocity(struct Player *self) {
    return self->velocity;
}

void player_set_velocity(struct Player *self, int value) {
    self->velocity = value;
}

void player_update(struct Player *self) {
    float xxx_prev_x = 0.0;
    float xxx_prev_y = 0.0;
    float distance = 20.0; //TODO: Change me.

    if (keys.right_pressed) {
        xxx_prev_x = player_get_x(&player);
        player_set_x(&player, xxx_prev_x + distance);
    }

    if (keys.left_pressed) {
        xxx_prev_x = player_get_x(&player);
        player_set_x(&player, xxx_prev_x - distance);
    }

    if (keys.up_pressed) {
        xxx_prev_y = player_get_y(&player);
        player_set_y(&player, xxx_prev_y - distance);
    }

    if (keys.down_pressed) {
        xxx_prev_y = player_get_y(&player);
        player_set_y(&player, xxx_prev_y + distance);
    }
}

void player_render(struct Player *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &self->rect);
}
// END NAV_Player


// NAV_ActorManager
void actor_manager_init(struct ActorManager *self, struct Actor *actors) {
    self->actors = actors;

    for (int i = 0; i < ACTOR_N; i++) {
        struct Actor *a = &self->actors[i];

        a->x = 0;
        a->y = 0;
        a->w = 20;
        a->h = 40;
        a->velocity = 0;
    }
}
// END NAV_ActorManager


// NAV_MiscFunctions
double performance_counters_to_ms(Uint64 start, Uint64 end) {
    double ms = (double)((end - start) * 1000) / SDL_GetPerformanceFrequency();

    return ms;
}
// END NAV_MiscFunctions


// NAV_Game
void game_run(struct Game *self, SDL_Renderer *renderer) {
    int keep_running = 1;

    Uint64 previous = SDL_GetPerformanceCounter();
    double lag = 0.0;
    float MS_PER_UPDATE = 16.00;

    while (keep_running) {
        Uint64 current = SDL_GetPerformanceCounter();
        double elapsed = performance_counters_to_ms(previous, current);
        previous = current;
        lag += elapsed;

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_running = 0;
            }

            input_component_update(&input_component, &event);
        }

        while (lag >= MS_PER_UPDATE) {
            player_update(&player);

            lag -= MS_PER_UPDATE;

            // SDL_Delay(xxx_loop_tick_sleep_ms - xxx_loop_tick_no_delay_ms);
        }

        // RENDER

        SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE); // grey

        int tmp = SDL_RenderClear(renderer);

        if (tmp != 0) {
            SDL_Log("ERROR: SDL_RenderClear() (%s)", SDL_GetError());
        }

        player_render(&player, renderer);

        SDL_RenderPresent(renderer);
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

    input_component_set_keys(&input_component, &keys);
    actor_manager_init(&actor_manager, actor_array);

    player_init(&player, 0, 0, 20, 40, 77);

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

// END NAV_Code
