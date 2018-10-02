#include <stdio.h>
#include <math.h>
#include "SDL.h"


// NAV_Config
#define WINDOW_W 256
#define WINDOW_H 512

#define UPDATES_PER_SECOND 120.0
#define MS_PER_UPDATE 8.33 // 1000 / 120.0
#define SLEEP_MS 2.08 // 1000 / 480.0

#define PLAYER_W 16
#define PLAYER_H 16
#define PLAYER_V 512
#define PLAYER_BULLETS_MAX 128
#define PLAYER_BULLETS_INIT_N 5
#define PLAYER_BULLETS_W 8
#define PLAYER_BULLETS_H 8
#define PLAYER_BULLETS_V 2048
#define PLAYER_BULLETS_VX +0
#define PLAYER_BULLETS_VY -1

#define ENEMY_W 32
#define ENEMY_H 32
#define ENEMY_V 128
#define ENEMIES_MAX 16
#define ENEMY_VX +0
#define ENEMY_VY +1

float PLAYER_BULLETS_OFFSET_X[] = { +0.0, -0.75, +0.75, -1.50, +1.50 };
float PLAYER_BULLETS_OFFSET_Y[] = { -1.0, -0.50, -0.50, +0.00, +0.00 };
// END NAV_Config


// NAV_Declarations

// NAV_Keys
struct Keys {
    int left;
    int right;
    int up;
    int down;
    int z;
};
// END NAV_Keys


// NAV_InputComponent
struct InputComponent {
};

void input_component_update(struct InputComponent *self, SDL_Event *event);
// END NAV_InputComponent


// NAV_Player
struct Player {
    struct BulletManager *bullet_manager;
    float x;
    float y;
    int w;
    int h;
    int v;
    int vx;
    int vy;
    int bullets_n;
    int firing;
    int fire_spacing;
    float fire_time;
    SDL_Rect rect;
};

void player_init(struct Player *self, struct BulletManager *bullet_manager, float x, float y, int w, int h, int v);
float player_get_x(struct Player *self);
void player_set_x(struct Player *self, float value);
float player_get_y(struct Player *self);
void player_set_y(struct Player *self, float value);
void player_on_button_a_keydown(struct Player *self);
void player_on_button_a_keyup(struct Player *self);
void player_fire(struct Player *self);
void player_update(struct Player *self);
void player_render(struct Player *self, SDL_Renderer *renderer);
// END NAV_Player


// NAV_Enemy
struct Enemy {
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

void enemy_init(struct Enemy *self, float x, float y, int w, int h, int v);
float enemy_get_x(struct Enemy *self);
void enemy_set_x(struct Enemy *self, float value);
float enemy_get_y(struct Enemy *self);
void enemy_set_vx(struct Enemy *self, float value);
void enemy_set_vy(struct Enemy *self, float value);
void enemy_set_y(struct Enemy *self, float value);
void enemy_update(struct Enemy *self);
void enemy_render(struct Enemy *self, SDL_Renderer *renderer);
// END NAV_Enemy


// NAV_EnemyManager
struct EnemyManager {
    struct Enemy *enemies;
    int n;
    float time;
    float spacing;
};

void enemy_manager_init(struct EnemyManager *self, struct Enemy *enemies, int n, float x, float y, int w, int h, int v);
struct Enemy *enemy_manager_get_free(struct EnemyManager *self);
void enemy_manager_spawn(struct EnemyManager *self);
void enemy_manager_update(struct EnemyManager *self);
void enemy_manager_render(struct EnemyManager *self, SDL_Renderer *renderer);
// ENDNAV_EnemyManager

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


// NAV_BulletManager
struct BulletManager {
    struct Bullet *bullets;
    int n;
};

void bullet_manager_init(struct BulletManager *self, struct Bullet *bullets, int n, float x, float y, int w, int h, int v);
struct Bullet *bullet_manager_get_free(struct BulletManager *self);
void bullet_manager_update(struct BulletManager *self);
void bullet_manager_render(struct BulletManager *self, SDL_Renderer *renderer);
// END NAV_BulletManager


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
struct Keys KEYS;
struct InputComponent INPUT_COMPONENT;
struct Player PLAYER;
struct Bullet PLAYER_BULLETS[PLAYER_BULLETS_MAX];
struct BulletManager PLAYER_BULLET_MANAGER;
struct Enemy ENEMIES[ENEMIES_MAX];
struct EnemyManager ENEMY_MANAGER;
struct Game GAME;

// END NAV_GlobalVariables


// NAV_Code

// NAV_InputComponent
void input_component_update(struct InputComponent *self, SDL_Event *event) {
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

            player_on_button_a_keydown(&PLAYER);
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

            player_on_button_a_keyup(&PLAYER);
        }
    }
}
// END NAV_InputComponent


// NAV_Player
void player_init(struct Player *self, struct BulletManager *bullet_manager, float x, float y, int w, int h, int v) {
    self->bullet_manager = bullet_manager;

    self->w = w;
    self->rect.w = self->w;

    self->h = h;
    self->rect.h = self->h;

    player_set_x(self, x);
    player_set_y(self, y);

    self->v = v;

    self->bullets_n = PLAYER_BULLETS_INIT_N;

    self->fire_spacing = 128;
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

void player_on_button_a_keydown(struct Player *self) {
    self->firing = 1;
    self->fire_time = self->fire_spacing * 1.0;
}

void player_on_button_a_keyup(struct Player *self) {
    self->firing = 0;
    self->fire_time = 0.0;
}

void player_fire(struct Player *self) {
    for (int i = 0; i < self->bullets_n; i++) {
        struct Bullet *bullet = bullet_manager_get_free(self->bullet_manager);

        if (bullet == NULL) {
            return;
        }

        bullet->alive = 1; //TODO: bullet_set_alive()

        bullet_set_x(bullet, self->x + self->w * PLAYER_BULLETS_OFFSET_X[i]);
        bullet_set_y(bullet, self->y + self->h * PLAYER_BULLETS_OFFSET_Y[i]);
        bullet_set_vx(bullet, PLAYER_BULLETS_VX);
        bullet_set_vy(bullet, PLAYER_BULLETS_VY);
    }
}

void player_update(struct Player *self) {
    self->vx = 0;
    self->vy = 0;

    if (KEYS.right) {
        self->vx = +1;
    }

    if (KEYS.left) {
        self->vx = -1;
    }

    if (KEYS.up) {
        self->vy = -1;
    }

    if (KEYS.down) {
        self->vy = +1;
    }

    player_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    player_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));

    if (KEYS.z) {
        self->fire_time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments
    }

    if (self->fire_time >= self->fire_spacing) {
        player_fire(self);
        self->fire_time = 0.0;
    }
}

void player_render(struct Player *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
// END NAV_Player

// NAV_Enemy
void enemy_init(struct Enemy *self, float x, float y, int w, int h, int v) {
    self->w = w;
    self->rect.w = self->w;

    self->h = h;
    self->rect.h = self->h;

    enemy_set_x(self, x);
    enemy_set_y(self, y);

    self->v = v;

    self->alive = 0;
}

float enemy_get_x(struct Enemy *self) {
    return self->x;
}

void enemy_set_x(struct Enemy *self, float value) {
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

float enemy_get_y(struct Enemy *self) {
    return self->y;
}

void enemy_set_y(struct Enemy *self, float value) {
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

void enemy_set_vx(struct Enemy *self, float value) {
    self->vx = value;
}

void enemy_set_vy(struct Enemy *self, float value) {
    self->vy = value;
}

void enemy_update(struct Enemy *self) {
    enemy_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    enemy_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));
}

void enemy_render(struct Enemy *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
// END NAV_Enemy


// NAV_EnemyManager
void enemy_manager_init(struct EnemyManager *self, struct Enemy *enemies, int n, float x, float y, int w, int h, int v) {
    self->enemies = enemies;
    self->n = n;

    self->time = 0.0;
    self->spacing = 1000.0;

    for (int i = 0; i < self->n; i++) {
        enemy_init(&self->enemies[i], x, y, w, h, v);
    }
}

struct Enemy *enemy_manager_get_free(struct EnemyManager *self) {
    struct Enemy *enemy = NULL;

    for (int i = 0; i < self->n; i++) {
        if (self->enemies[i].alive == 1) {
            continue;
        }

        enemy = &self->enemies[i];

        break;
    }

    return enemy;
}

void enemy_manager_spawn(struct EnemyManager *self) {
    struct Enemy *enemy = enemy_manager_get_free(self);

    enemy->alive = 1; //TODO: enemy_set_alive()

    enemy_set_x(enemy, 50);
    enemy_set_y(enemy, 0);
    enemy_set_vx(enemy, ENEMY_VX);
    enemy_set_vy(enemy, ENEMY_VY);
}

void enemy_manager_update(struct EnemyManager *self) {
    for (int i = 0; i < self->n; i++) {
        if (!self->enemies[i].alive) {
            continue;
        }

        enemy_update(&self->enemies[i]);
    }

    self->time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments

    if (self->time >= self->spacing) {
        enemy_manager_spawn(self);
        self->time = 0.0;
    }
}

void enemy_manager_render(struct EnemyManager *self, SDL_Renderer *renderer) {
    for (int i = 0; i < self->n; i++) {
        if (!self->enemies[i].alive) {
            continue;
        }

        enemy_render(&self->enemies[i], renderer);
    }
}
// END NAV_EnemyManager


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
    bullet_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    bullet_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));
}

void bullet_render(struct Bullet *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
// END NAV_Bullet


// NAV_BulletManager
void bullet_manager_init(struct BulletManager *self, struct Bullet *bullets, int n, float x, float y, int w, int h, int v) {
    self->bullets = bullets;
    self->n = n;

    for (int i = 0; i < n; i++) {
        bullet_init(&bullets[i], 0, 0, w, h, v);
    }
}

struct Bullet *bullet_manager_get_free(struct BulletManager *self) {
    struct Bullet *bullet = NULL;

    for (int i = 0; i < self->n; i++) {
        if (self->bullets[i].alive == 1) {
            continue;
        }

        bullet = &self->bullets[i];

        break;
    }

    return bullet;
}

void bullet_manager_update(struct BulletManager *self) {
    for (int i = 0; i < self->n; i++) {
        if (!self->bullets[i].alive) {
            continue;
        }

        bullet_update(&self->bullets[i]);
    }
}

void bullet_manager_render(struct BulletManager *self, SDL_Renderer *renderer) {
    for (int i = 0; i < self->n; i++) {
        if (!self->bullets[i].alive) {
            continue;
        }

        bullet_render(&self->bullets[i], renderer);
    }
}
// END NAV_BulletManager


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

            input_component_update(&INPUT_COMPONENT, &event);
        }

        while (lag >= MS_PER_UPDATE) {
            player_update(&PLAYER);

            bullet_manager_update(&PLAYER_BULLET_MANAGER);

            enemy_manager_update(&ENEMY_MANAGER);

            lag -= MS_PER_UPDATE;
        }

        // RENDER

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // grey
        SDL_RenderClear(renderer);

        player_render(&PLAYER, renderer);

        bullet_manager_render(&PLAYER_BULLET_MANAGER, renderer);

        enemy_manager_render(&ENEMY_MANAGER, renderer);

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

    bullet_manager_init(&PLAYER_BULLET_MANAGER, PLAYER_BULLETS, PLAYER_BULLETS_MAX, 0, 0, PLAYER_BULLETS_W, PLAYER_BULLETS_H, PLAYER_BULLETS_V);
    player_init(&PLAYER, &PLAYER_BULLET_MANAGER, WINDOW_W / 2, WINDOW_H / 2, PLAYER_W, PLAYER_H, PLAYER_V);

    enemy_manager_init(&ENEMY_MANAGER, ENEMIES, ENEMIES_MAX, 0, 0, ENEMY_W, ENEMY_H, ENEMY_V);

    game_run(&GAME, renderer);

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
