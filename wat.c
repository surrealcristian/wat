#include <stdio.h>
#include <math.h>
#include "SDL.h"

#define UPDATES_PER_SECOND 120.0
#define MS_PER_UPDATE 8.33
#define SLEEP_MS 2.08
#define WINDOW_W 480
#define WINDOW_H 640

#define PLAYER_BULLETS_N 20


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
    int z_pressed;
};
// END NAV_Keys


// NAV_InputComponent
struct InputComponent {
    struct Keys *keys;
};

void input_component_init(struct InputComponent *self, struct Keys *keys);
void input_component_update(struct InputComponent *self, SDL_Event *event);
// END NAV_InputComponent


// NAV_Player
struct Player {
    float x;
    float y;
    int w;
    int h;
    int v;
    int vx;
    int vy;
    int shooting;
    float shoot_interval;
    float shoot_time;
    SDL_Rect rect;
};

void player_init(struct Player *self, float x, float y, int w, int h, int v);
float player_get_x(struct Player *self);
void player_set_x(struct Player *self, float value);
float player_get_y(struct Player *self);
void player_set_y(struct Player *self, float value);
void player_on_a_keydown(struct Player *self);
void player_on_a_keyup(struct Player *self);
void player_shoot(struct Player *self);
void player_update(struct Player *self);
void player_render(struct Player *self, SDL_Renderer *renderer);
// END NAV_Player

// NAV_Bullet
struct Bullet {
    float x;
    float y;
    int w;
    int h;
    int v;
    int vx;
    int vy;
    int alive;
    SDL_Rect rect;
};

void bullet_init(struct Bullet *self, float x, float y, int w, int h, int v);
float bullet_get_x(struct Bullet *self);
void bullet_set_x(struct Bullet *self, float value);
float bullet_get_y(struct Bullet *self);
void bullet_set_y(struct Bullet *self, float value);
int bullet_get_v(struct Bullet *self);
void bullet_set_vx(struct Bullet *self, int value);
void bullet_set_vy(struct Bullet *self, int value);
void bullet_update(struct Bullet *self);
void bullet_render(struct Bullet *self, SDL_Renderer *renderer);
// ENV NAV_Bullet


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
struct Player player;
struct Bullet player_bullets[PLAYER_BULLETS_N];
struct Game game;
// END NAV_GlobalVariables


// NAV_Code

// NAV_InputComponent
void input_component_init(struct InputComponent *self, struct Keys *keys) {
    self->keys = keys;
}

void input_component_update(struct InputComponent *self, SDL_Event *event) {
    int sym;

    if (event->type == SDL_KEYDOWN) {
        if (event->key.repeat) {
            return;
        }

        //SDL_Log("INFO: SDL_KEYDOWN");

        sym = event->key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            self->keys->right_pressed = 1;
        } else if (sym == SDLK_LEFT) {
            self->keys->left_pressed = 1;
        } else if (sym == SDLK_UP) {
            self->keys->up_pressed = 1;
        } else if (sym == SDLK_DOWN) {
            self->keys->down_pressed = 1;
        } else if (sym == SDLK_z) {
            self->keys->z_pressed = 1;

            player_on_a_keydown(&player);

            SDL_Log("SHOOT"); //TODO: delete
        }
    } else if (event->type == SDL_KEYUP) {
        if (event->key.repeat) {
            return;
        }

        //SDL_Log("INFO: SDL_KEYUP");

        sym = event->key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            self->keys->right_pressed = 0;
        } else if (sym == SDLK_LEFT) {
            self->keys->left_pressed = 0;
        } else if (sym == SDLK_UP) {
            self->keys->up_pressed = 0;
        } else if (sym == SDLK_DOWN) {
            self->keys->down_pressed = 0;
        } else if (sym == SDLK_z) {
            self->keys->z_pressed = 0;

            player_on_a_keyup(&player);

            SDL_Log("DON'T SHOOT"); //TODO: delete
        }
    }
}
// END NAV_InputComponent


// NAV_Player
void player_init(struct Player *self, float x, float y, int w, int h, int v) {
    self->w = w;
    self->rect.w = self->w;

    self->h = h;
    self->rect.h = self->h;

    player_set_x(self, x);
    player_set_y(self, y);

    self->v = v;

    self->shoot_interval = 100.0;
}

float player_get_x(struct Player *self) {
    return self->x;
}

void player_set_x(struct Player *self, float value) {
    float x_min = 0 + self->w / 2;
    float x_max = WINDOW_W - self->w / 2;

    if (value < x_min) {
        value = x_min;
    } else if (value > x_max) {
        value = x_max;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

float player_get_y(struct Player *self) {
    return self->y;
}

void player_set_y(struct Player *self, float value) {
    float y_min = 0 + self->h / 2;
    float y_max = WINDOW_H - self->h / 2;

    if (value < y_min) {
        value = y_min;
    } else if (value > y_max) {
        value = y_max;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

void player_on_a_keydown(struct Player *self) {
    self->shooting = 1;
    self->shoot_time = self->shoot_interval;
}

void player_on_a_keyup(struct Player *self) {
    self->shooting = 0;
    self->shoot_time = 0.0;
}

void player_shoot(struct Player *self) {
    int bullet_i = -1;

    for (int i = 0; i < PLAYER_BULLETS_N; i++) {
        if (player_bullets[i].alive == 1) {
            continue;
        }

        bullet_i = i;

        break;
    }

    if (bullet_i == -1) {
        return;
    }

    struct Bullet *bullet = &player_bullets[bullet_i];

    bullet->alive = 1;

    int v = bullet_get_v(bullet);

    bullet_set_x(bullet, self->x);
    bullet_set_y(bullet, self->y + 10);
    bullet_set_vx(bullet, 0);
    bullet_set_vy(bullet, -v);
}

void player_update(struct Player *self) {
    self->vx = 0;
    self->vy = 0;

    if (keys.right_pressed) {
        self->vx += self->v;
    }

    if (keys.left_pressed) {
        self->vx -= self->v;
    }

    if (keys.up_pressed) {
        self->vy -= self->v;
    }

    if (keys.down_pressed) {
        self->vy += self->v;
    }

    player_set_x(self, self->x + (1.0 * self->vx / UPDATES_PER_SECOND));
    player_set_y(self, self->y + (1.0 * self->vy / UPDATES_PER_SECOND));

    if (keys.z_pressed) {
        self->shoot_time += MS_PER_UPDATE;
    }

    if (self->shoot_time >= self->shoot_interval) {
        player_shoot(self);
        self->shoot_time = 0.0;
    }
}

void player_render(struct Player *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
// END NAV_Player
//
// NAV_Bullet
void bullet_init(struct Bullet *self, float x, float y, int w, int h, int v) {
    self->w = w;
    self->rect.w = self->w;

    self->h = h;
    self->rect.h = self->h;

    bullet_set_x(self, x);
    bullet_set_y(self, y);

    self->v = v;

    self->alive = 0;
}

float bullet_get_x(struct Bullet *self) {
    return self->x;
}

void bullet_set_x(struct Bullet *self, float value) {
    float x_min = 0;
    float x_max = WINDOW_W;

    if (value < x_min || value > x_max) {
        self->alive = 0;
        return;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

float bullet_get_y(struct Bullet *self) {
    return self->y;
}

void bullet_set_y(struct Bullet *self, float value) {
    float y_min = 0;
    float y_max = WINDOW_H;

    if (value < y_min || value > y_max) {
        self->alive = 0;
        return;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

int bullet_get_v(struct Bullet *self) {
    return self->v;
}

void bullet_set_vx(struct Bullet *self, int value) {
    self->vx = value;
}

void bullet_set_vy(struct Bullet *self, int value) {
    self->vy = value;
}

void bullet_update(struct Bullet *self) {
    bullet_set_x(self, self->x + (1.0 * self->vx / UPDATES_PER_SECOND));
    bullet_set_y(self, self->y + (1.0 * self->vy / UPDATES_PER_SECOND));
}

void bullet_render(struct Bullet *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
// END NAV_Bullet


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

            input_component_update(&input_component, &event);
        }

        while (lag >= MS_PER_UPDATE) {
            player_update(&player);

            for (int i = 0; i < PLAYER_BULLETS_N; i++) {
                if (!player_bullets[i].alive) {
                    continue;
                }

                bullet_update(&player_bullets[i]);
            }

            lag -= MS_PER_UPDATE;
        }

        // RENDER

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // grey

        int tmp = SDL_RenderClear(renderer);

        if (tmp != 0) {
            SDL_Log("ERROR: SDL_RenderClear() (%s)", SDL_GetError());
        }

        player_render(&player, renderer);

        for (int i = 0; i < PLAYER_BULLETS_N; i++) {
            if (!player_bullets[i].alive) {
                continue;
            }

            bullet_render(&player_bullets[i], renderer);
        }

        SDL_RenderPresent(renderer);

        //SDL_Delay(SLEEP_MS);
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
        //SDL_RENDERER_ACCELERATED
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
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

    input_component_init(&input_component, &keys);

    player_init(&player, WINDOW_W / 2, WINDOW_H / 2, 20, 20, 512);

    for (int i = 0; i < PLAYER_BULLETS_N; i++) {
        bullet_init(&player_bullets[i], 0, 0, 20, 20, 2048);
    }

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
