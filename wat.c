#include "wat.h"

struct Entity PLAYERS[PLAYER_MAX];
struct Entity ENEMIES[ENEMY_MAX];
struct Entity BULLETS[BULLET_MAX];
struct Entity PARTICLES[PARTICLE_MAX];

tinymt32_t          TINYMT_STATE;
SDL_Event           EVENT;
struct Keyboard     KEYBOARD;
struct EnemyManager ENEMY_MANAGER;
struct Score        SCORE;
struct Text         WELCOME_TEXT;
struct Text         HUD_TEXT;
struct HUD          HUD;
struct Text         PAUSE_TEXT;
struct Game         GAME;



/* util.c start */
double performance_counters_to_ms(Uint64 start, Uint64 end) {
    double ms = (double)((end - start) * 1000) / SDL_GetPerformanceFrequency();

    return ms;
}

void rand_init(tinymt32_t *state, uint32_t seed) {
    tinymt32_init(state, seed);
}

int rand_n( tinymt32_t *state, int n) {
    uint32_t rn = tinymt32_generate_uint32(state);
    int ret = (int)(rn >> 5) % n;

    return ret;
}

float fclamp(float f, float min, float max) {
    return fmin(fmax(f, min), max);
}
/* util.c end */


/* vec2f.c start */
void vec2f_set_xy(struct Vec2f *self, float x, float y) {
    self->x = x;
    self->y = y;
}

float vec2f_length(struct Vec2f *v) {
    float length = sqrt((v->x * v->x) + (v->y * v->y));

    return length;
}

void vec2f_normalize(struct Vec2f *self) {
    float length = vec2f_length(self);

    self->x /= length;
    self->y /= length;
}
/* vec2f.c end */


/* text.c start */
float RUNE_X_POSITIONS[25] = { 0,1,2,3,4, 0,1,2,3,4, 0,1,2,3,4, 0,1,2,3,4, 0,1,2,3,4, };
float RUNE_Y_POSITIONS[25] = { 0,0,0,0,0, 1,1,1,1,1, 2,2,2,2,2, 3,3,3,3,3, 4,4,4,4,4, };

int RUNES[36][25] = {
    /* 0 */ { 0,1,1,1,0, 1,0,0,1,1, 1,0,1,0,1, 1,1,0,0,1, 0,1,1,1,0, },
    /* 1 */ { 0,0,1,0,0, 1,1,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 1,1,1,1,1, },
    /* 2 */ { 0,1,1,1,0, 1,0,0,0,1, 0,0,1,1,0, 0,1,0,0,0, 1,1,1,1,1, },
    /* 3 */ { 1,1,1,1,0, 0,0,0,0,1, 0,0,1,1,0, 0,0,0,0,1, 1,1,1,1,0, },
    /* 4 */ { 0,0,0,1,0, 0,0,1,1,0, 1,1,0,1,0, 1,1,1,1,1, 0,0,0,1,0, },
    /* 5 */ { 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,0, 0,0,0,0,1, 1,1,1,1,0, },
    /* 6 */ { 0,1,1,1,0, 1,0,0,0,0, 1,1,1,1,0, 1,0,0,0,1, 0,1,1,1,0, },
    /* 7 */ { 1,1,1,1,1, 0,0,0,0,1, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, },
    /* 8 */ { 0,1,1,1,0, 1,0,0,0,1, 0,1,1,1,0, 1,0,0,0,1, 0,1,1,1,0, },
    /* 9 */ { 0,1,1,1,0, 1,0,0,0,1, 0,1,1,1,1, 0,0,0,0,1, 0,1,1,1,0, },

    /* A */ { 0,1,1,1,0, 1,0,0,0,1, 1,1,1,1,1, 1,0,0,0,1, 1,0,0,0,1, },
    /* B */ { 1,1,1,1,0, 1,0,0,0,1, 1,1,1,1,0, 1,0,0,0,1, 1,1,1,1,0, },
    /* C */ { 0,1,1,1,0, 1,0,0,0,1, 1,0,0,0,0, 1,0,0,0,1, 0,1,1,1,0, },
    /* D */ { 1,1,1,1,0, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 1,1,1,1,0, },
    /* E */ { 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,0, 1,0,0,0,0, 1,1,1,1,1, },
    /* F */ { 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,0, 1,0,0,0,0, 1,0,0,0,0, },
    /* G */ { 0,1,1,1,0, 1,0,0,0,0, 1,0,0,1,1, 1,0,0,0,1, 0,1,1,1,0, },
    /* H */ { 1,0,0,0,1, 1,0,0,0,1, 1,1,1,1,1, 1,0,0,0,1, 1,0,0,0,1, },
    /* I */ { 1,1,1,1,1, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 1,1,1,1,1, },
    /* J */ { 0,1,1,1,1, 0,0,0,0,1, 0,0,0,0,1, 1,0,0,0,1, 0,1,1,1,0, },

    /* K */ { 1,0,0,0,1, 1,0,0,1,0, 1,1,1,0,0, 1,0,0,1,0, 1,0,0,0,1, },
    /* L */ { 1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 1,0,0,0,0, 1,1,1,1,1, },
    /* M */ { 1,0,0,0,1, 1,1,0,1,1, 1,0,1,0,1, 1,0,1,0,1, 1,0,0,0,1, },
    /* N */ { 1,0,0,0,1, 1,1,0,0,1, 1,0,1,0,1, 1,0,0,1,1, 1,0,0,0,1, },
    /* O */ { 0,1,1,1,0, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 0,1,1,1,0, },
    /* P */ { 1,1,1,1,0, 1,0,0,0,1, 1,1,1,1,0, 1,0,0,0,0, 1,0,0,0,0, },
    /* Q */ { 0,1,1,1,0, 1,0,0,0,1, 1,0,1,0,1, 1,0,0,1,0, 0,1,1,0,1, },
    /* R */ { 1,1,1,1,0, 1,0,0,0,1, 1,1,1,1,0, 1,0,0,1,0, 1,0,0,0,1, },
    /* S */ { 0,1,1,1,1, 1,0,0,0,0, 0,1,1,1,0, 0,0,0,0,1, 1,1,1,1,0, },
    /* T */ { 1,1,1,1,1, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, },

    /* U */ { 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 0,1,1,1,0, },
    /* V */ { 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, },
    /* W */ { 1,0,0,0,1, 1,0,0,0,1, 1,0,1,0,1, 1,1,0,1,1, 1,0,0,0,1, },
    /* X */ { 1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, 0,1,0,1,0, 1,0,0,0,1, },
    /* Y */ { 1,0,0,0,1, 1,0,0,0,1, 0,1,0,1,0, 0,0,1,0,0, 0,0,1,0,0, },
    /* Z */ { 1,1,1,1,1, 0,0,0,1,0, 0,0,1,0,0, 0,1,0,0,0, 1,1,1,1,1, },
};

void text_init(
    struct Text *self,
    char        *value,
    int         size,
    int         align,
    float       x,
    float       y
) {
    self->value = value;
    self->size  = size;
    self->align = align;
    self->x     = x;
    self->y     = y;

    if (self->size == TEXT_SIZE_SMALL) {
        self->size_px = RUNE_SMALL_PX;
    } else if (self->size == TEXT_SIZE_MEDIUM) {
        self->size_px = RUNE_MEDIUM_PX;
    } else if (self->size == TEXT_SIZE_LARGE) {
        self->size_px = RUNE_LARGE_PX;
    }

    self->rect.w = self->size_px;
    self->rect.h = self->size_px;

    self->value_len = strlen(self->value);
}

void text_set_value(struct Text *self, char *value) {
    self->value = value;
    self->value_len = strlen(value);
}

void text_set_size(struct Text *self, int size) {
    self->size = size;
}

void text_set_align(struct Text *self, int align) {
    self->align = align;
}

void text_set_x(struct Text *self, float x) {
    self->x = x;
}

void text_set_y( struct Text *self, float y) {
    self->y = y;
}

void text_render_rune(
    struct Text  *self,
    SDL_Renderer *renderer,
    int          rune,
    float        x,
    float        y
) {
    for (int i = 0; i < 25; i++) {
        if (RUNES[rune][i] == 0) {
            continue;
        }

        self->rect.x = x + RUNE_X_POSITIONS[i] * self->size_px;
        self->rect.y = y + RUNE_Y_POSITIONS[i] * self->size_px;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &self->rect);
    }
}

void text_render(struct Text  *self, SDL_Renderer *renderer) {
    int   rune_w_px       = RUNE_W * self->size_px;
    int   rune_h_px       = RUNE_H * self->size_px;
    int   rune_space_w_px = RUNE_SPACE_W * self->size_px;
    float text_w_px       = (self->value_len * rune_w_px) + ((self->value_len - 1) * rune_space_w_px);
    float rune_x          = 0.0;
    float rune_y          = 0.0;
    int   c;
    int   idx;

    if (self->align == TEXT_ALIGN_LEFT) {
        rune_x = self->x;
    } else if (self->align == TEXT_ALIGN_CENTER) {
        rune_x = self->x - (text_w_px / 2);
    } else if (self->align == TEXT_ALIGN_RIGHT) {
        rune_x = self->x - text_w_px;
    }

    rune_y = self->y - (rune_h_px / 2);


    for (int i = 0; self->value[i] != '\0'; i++) {
        c = self->value[i];

        if (c >= '0' && c <= '9') {
            idx = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            idx = c - 'A' + 10;
        } else {
            return;
        }

        text_render_rune(self, renderer, idx, rune_x, rune_y);

        rune_x += rune_w_px + rune_space_w_px;
    }
}
/* text.c end */


/* config.c start */
float PLAYER_BULLETS_OFFSET_X[5] = { +0.00, -0.75, +0.75, -1.50, +1.50 };
float PLAYER_BULLETS_OFFSET_Y[5] = { -1.00, -0.50, -0.50, +0.00, +0.00 };

float EXPLOSION_PARTICLES_VX[4] = { -1.00, +1.00, -1.00, +1.00 };
float EXPLOSION_PARTICLES_VY[4] = { -1.00, -1.00, +1.00, +1.00 };

struct Vec2f EXPLOSION_PARTICLES_DIR[4] = {
    { .x = -1.0, .y = -1.0 },
    { .x = +1.0, .y = -1.0 },
    { .x = -1.0, .y = +1.0 },
    { .x = +1.0, .y = +1.0 },
};
/* config.c end */


/* player.c start */
void player_on_button_a_keydown(struct Entity *entity) {
    entity->sho_firing = 1;
    entity->sho_fire_time = entity->sho_fire_spacing * 1.0;
}

void player_on_button_a_keyup(struct Entity *entity) {
    entity->sho_firing = 0;
    entity->sho_fire_time = 0.0;
}

void player_fire(struct Entity *player) {
    for (int i = 0; i < player->sho_bullets_n; i++) {
        struct Entity *bullet = health_get_dead_range(BULLETS, BULLET_MAX);

        if (bullet == NULL) {
            return;
        }

        bullet->hea_alive = 1;

        float x = player->pos_pos.x + player->pos_w * PLAYER_BULLETS_OFFSET_X[i];
        float y = player->pos_pos.y + player->pos_h * PLAYER_BULLETS_OFFSET_Y[i];

        bullet->pos_pos.x = x;
        bullet->pos_pos.y = y;

        vec2f_set_xy(&bullet->mov_dir, +0.0, -1.0);
    }
}

void player_fire_update(struct Entity *entity) {
    if (KEYBOARD.z) {
        entity->sho_fire_time += MS_PER_UPDATE; /* TODO: move MS_PER_UPDATE to arguments */
    }

    if (entity->sho_fire_time >= entity->sho_fire_spacing) {
        player_fire(&PLAYERS[0]);
        entity->sho_fire_time = 0.0;
    }
}

void player_fire_update_all() {
    for (int i = 0; i < PLAYER_MAX; i++) {
        struct Entity *player = &PLAYERS[i];

        if (player->hea_alive == 0) {
            continue;
        }

        player_fire_update(player);
    }
}
/* player.c end */


/* enemy.c start */
void enemy_try_spawn() {
    ENEMY_MANAGER.time += MS_PER_UPDATE; /* TODO: move MS_PER_UPDATE to arguments */

    if (ENEMY_MANAGER.time >= ENEMY_MANAGER.spacing) {
        enemy_spawn();
        ENEMY_MANAGER.time = 0.0;
    }
}

void enemy_spawn() {
    struct Entity *enemy = health_get_dead_range(ENEMIES, ENEMY_MAX);

    if (enemy == NULL) {
        return;
    }

    enemy->hea_alive = 1;

    enemy->pos_pos.x = rand_n(&TINYMT_STATE, WINDOW_W + 1);
    enemy->pos_pos.y = 0 - enemy->pos_h;

    vec2f_set_xy(&enemy->mov_dir, +0.0, +1.0);
}
/* enemy.c end */


/* score.c start */
void score_init() {
    SCORE.value = 0;
}
/* score.c end */


/* collision.c start */
void collision_player_vs_enemies() {
    struct Entity *player = &PLAYERS[0];

    if (player->hea_alive == 0) {
        return;
    }

    for (int i = 0; i < ENEMY_MAX; i++) {
        struct Entity *enemy = &ENEMIES[i];

        if (enemy->hea_alive == 0) {
            continue;
        }

        if (SDL_HasIntersection(&player->col_sdl_rect, &enemy->col_sdl_rect) == SDL_TRUE) {
            enemy->hea_alive = 0;
            continue;
        }
    }
}

void collision_enemies_vs_player_bullets() {
    for (int i = 0; i < ENEMY_MAX; i++) {
        struct Entity *enemy = &ENEMIES[i];

        if (enemy->hea_alive == 0) {
            continue;
        }

        for (int j = 0; j < BULLET_MAX; j++) {
            struct Entity *bullet = &BULLETS[j];

            if (bullet->hea_alive == 0) {
                continue;
            }

            if (SDL_HasIntersection(&enemy->col_sdl_rect, &bullet->col_sdl_rect) == SDL_TRUE) {
                enemy->hea_alive = 0;
                bullet->hea_alive = 0;

                collision_make_explosion(enemy->pos_pos.x, enemy->pos_pos.y);

                SCORE.value += ENEMY_SCORE;

                break;
            }
        }
    }
}

void collision_update() {
    collision_player_vs_enemies();
    collision_enemies_vs_player_bullets();
}

void collision_make_explosion(float x, float y) {
    for (int i = 0; i < EXPLOSION_PARTICLES_N; i++) {
        struct Entity *particle = health_get_dead_range(PARTICLES, PARTICLE_MAX);

        if (particle == NULL) {
            return;
        }

        particle->pos_pos.x = x;
        particle->pos_pos.y = y;

        particle->mov_dir = EXPLOSION_PARTICLES_DIR[i];

        particle->hea_alive = 1;
    }
}
/* collision.c end */


/* welcome_state.c start */
void welcome_state_update() {

}

void welcome_state_render(SDL_Renderer *renderer) {
    text_render(&WELCOME_TEXT, renderer);
}
/* welcome_state.c end */


/* hud.c start */
char HUD_SCORE_TEXT[HUD_SCORE_TEXT_BUFSIZE];

void hud_update() {
    snprintf(HUD_SCORE_TEXT, HUD_SCORE_TEXT_BUFSIZE, "%lu", SCORE.value);

    text_set_value(&HUD_TEXT, HUD_SCORE_TEXT);
}

void hud_render(SDL_Renderer *renderer) {
    text_render(&HUD_TEXT, renderer);
}
/* hud.c end */


/* in_game_state.c start */
void in_game_state_update() {
    struct Entity *player = &PLAYERS[0];

    float tmp_x = +0.0;
    float tmp_y = +0.0;

    if (KEYBOARD.right) {
        tmp_x += +1.0;
    }

    if (KEYBOARD.left) {
        tmp_x += -1.0;
    }

    if (KEYBOARD.up) {
        tmp_y += -1.0;
    }

    if (KEYBOARD.down) {
        tmp_y += +1.0;
    }

    vec2f_set_xy(&player->mov_dir, tmp_x, tmp_y);

    movement_update_range(PLAYERS, PLAYER_MAX);
    movement_update_range(ENEMIES, ENEMY_MAX);
    movement_update_range(BULLETS, BULLET_MAX);
    movement_update_range(PARTICLES, PARTICLE_MAX);

    //xxx

    player_fire_update_all();

    movement_fclamp_player(player);

    health_kill_if_out_of_map_range(BULLETS, BULLET_MAX);
    health_kill_if_out_of_map_range(ENEMIES, ENEMY_MAX);
    health_kill_if_out_of_map_range(PARTICLES, PARTICLE_MAX);

    collision_sync_range(PLAYERS, PLAYER_MAX);
    collision_sync_range(ENEMIES, ENEMY_MAX);
    collision_sync_range(BULLETS, BULLET_MAX);

    collision_update();

    enemy_try_spawn();

    hud_update();
}

void in_game_state_render(SDL_Renderer *renderer) {
    render_sync_range(PARTICLES, PARTICLE_MAX);
    render_sync_range(PLAYERS, PLAYER_MAX);
    render_sync_range(ENEMIES, ENEMY_MAX);
    render_sync_range(BULLETS, BULLET_MAX);

    render_update_range(PLAYERS, PLAYER_MAX, renderer);
    render_update_range(ENEMIES, ENEMY_MAX, renderer);
    render_update_range(BULLETS, BULLET_MAX, renderer);
    render_update_range(PARTICLES, PARTICLE_MAX, renderer);

    hud_render(renderer);
}
/* in_game_state.c end */


/* pause_state.c start */
void pause_state_update() {

}

void pause_state_render(SDL_Renderer *renderer) {
    render_sync_range(PARTICLES, PARTICLE_MAX);
    render_sync_range(PLAYERS, PLAYER_MAX);
    render_sync_range(ENEMIES, ENEMY_MAX);
    render_sync_range(BULLETS, BULLET_MAX);

    render_update_range(PLAYERS, PLAYER_MAX, renderer);
    render_update_range(ENEMIES, ENEMY_MAX, renderer);
    render_update_range(BULLETS, BULLET_MAX, renderer);
    render_update_range(PARTICLES, PARTICLE_MAX, renderer);

    hud_render(renderer);

    text_render(&PAUSE_TEXT, renderer);
}
/* pause_state.c end */


/* game.c start */
void game_init() {
    GAME.state = STATE_WELCOME;
}

void game_run(SDL_Renderer *renderer) {
    int keep_running = 1;

    Uint64 debug_start;
    Uint64 debug_end;
    double debug_elapsed;

    Uint64 current;
    double elapsed;
    double lag = 0.0;

    Uint64 previous = SDL_GetPerformanceCounter();

    while (keep_running) {
        /* ----- */
        debug_start = SDL_GetPerformanceCounter();
        /* ----- */

        current = SDL_GetPerformanceCounter();
        elapsed = performance_counters_to_ms(previous, current);

        previous = current;
        lag += elapsed;

        while (SDL_PollEvent(&EVENT)) {
            if (EVENT.type == SDL_QUIT) {
                keep_running = 0;
            }

            input_update();
        }

        while (lag >= MS_PER_UPDATE) {
            if (GAME.state == STATE_WELCOME) {
                welcome_state_update();
            } else if (GAME.state == STATE_IN_GAME) {
                in_game_state_update();
            } else if (GAME.state == STATE_PAUSE) {
                pause_state_update();
            }

            lag -= MS_PER_UPDATE;
        }

        /* RENDER */

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        if (GAME.state == STATE_WELCOME) {
            welcome_state_render(renderer);
        } else if (GAME.state == STATE_IN_GAME) {
            in_game_state_render(renderer);
        } else if (GAME.state == STATE_PAUSE) {
            pause_state_render(renderer);
        }

        /* ----- */
        debug_end = SDL_GetPerformanceCounter();
        debug_elapsed = performance_counters_to_ms(debug_start, debug_end);
        if (GAME.state == STATE_IN_GAME) {
            SDL_Log("%f", debug_elapsed);
        }
        /* ----- */

        SDL_RenderPresent(renderer);

        /* SDL_Delay(SLEEP_MS); */
    }
}
/* game.c end */


/* input.c start */
void input_update() {
    int sym;

    struct Entity *player = &PLAYERS[0];

    if (EVENT.type == SDL_KEYDOWN) {
        if (EVENT.key.repeat) {
            return;
        }

        sym = EVENT.key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            KEYBOARD.right = 1;
        } else if (sym == SDLK_LEFT) {
            KEYBOARD.left = 1;
        } else if (sym == SDLK_UP) {
            KEYBOARD.up = 1;
        } else if (sym == SDLK_DOWN) {
            KEYBOARD.down = 1;
        } else if (sym == SDLK_z) {
            KEYBOARD.z = 1;

            if (GAME.state == STATE_IN_GAME) {
                player_on_button_a_keydown(player);
            }
        }
    } else if (EVENT.type == SDL_KEYUP) {
        if (EVENT.key.repeat) {
            return;
        }

        sym = EVENT.key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            KEYBOARD.right = 0;
        } else if (sym == SDLK_LEFT) {
            KEYBOARD.left = 0;
        } else if (sym == SDLK_UP) {
            KEYBOARD.up = 0;
        } else if (sym == SDLK_DOWN) {
            KEYBOARD.down = 0;
        } else if (sym == SDLK_z) {
            KEYBOARD.z = 0;

            if (GAME.state == STATE_WELCOME) {
                GAME.state = STATE_IN_GAME;
            } else if (GAME.state == STATE_IN_GAME) {
                player_on_button_a_keyup(player);
            }
        } else if (sym == SDLK_ESCAPE) {
            if (GAME.state == STATE_IN_GAME) {
                GAME.state = STATE_PAUSE;
            } else if (GAME.state == STATE_PAUSE) {
                GAME.state = STATE_IN_GAME;
            }
        }
    }
}
/* input.c end */


void movement_init(struct Entity *entity, float x, float y, int w, int h, int v) {
    entity->pos_w = w;
    entity->pos_h = h;

    entity->pos_pos.x = x;
    entity->pos_pos.y = y;

    entity->mov_vel = v;
}

void movement_fclamp_player(struct Entity *entity) {
    float x_min = 0 + entity->pos_w / 2;
    float x_max = WINDOW_W - entity->pos_w / 2;
    float y_min = 0 + entity->pos_h / 2;
    float y_max = WINDOW_H - entity->pos_h / 2;

    entity->pos_pos.x = fclamp(entity->pos_pos.x, x_min, x_max);
    entity->pos_pos.y = fclamp(entity->pos_pos.y, y_min, y_max);

}

void movement_update(struct Entity *entity) {
    if (entity->mov_dir.x != 0.0 || entity->mov_dir.y != 0.0) {
        vec2f_normalize(&entity->mov_dir);
        
        entity->pos_pos.x += (entity->mov_vel * entity->mov_dir.x) / UPDATES_PER_SECOND;
        entity->pos_pos.y += (entity->mov_vel * entity->mov_dir.y) / UPDATES_PER_SECOND;
    }
}

void movement_update_range(struct Entity *entities, int n) {
    for (int i = 0; i < n; i++) {
        struct Entity *entity = &entities[i];

        if (entity->hea_alive == 0) {
            continue;
        }

        movement_update(entity);
    }
}

void collision_sync_range(struct Entity *entities, int n) {
    for (int i = 0; i < n; i++) {
        struct Entity *entity = &entities[i];

        if (entity->hea_alive == 0) {
            continue;
        }

        entity->col_sdl_rect.w = entity->pos_w;
        entity->col_sdl_rect.h = entity->pos_h;

        entity->col_sdl_rect.x = floor(entity->pos_pos.x - (entity->pos_w / 2));
        entity->col_sdl_rect.y = floor(entity->pos_pos.y - (entity->pos_h / 2));
    }
}

void health_init(struct Entity *entity, int alive) {
    entity->hea_alive = alive;
}

void health_kill_if_out_of_range(struct Entity *entity, float xmin, float xmax, float ymin, float ymax) {
    if (entity->pos_pos.x < xmin || entity->pos_pos.x > xmax) {
        entity->hea_alive = 0;

        entity->pos_pos.x = 0;
        entity->pos_pos.y = 0;

        return;
    }

    if (entity->pos_pos.y < ymin || entity->pos_pos.y > ymax) {
        entity->hea_alive = 0;

        entity->pos_pos.x = 0;
        entity->pos_pos.y = 0;

        return;
    }
}

void health_kill_if_out_of_map_range(struct Entity *entities, int n) {
    for (int i = 0; i < n; i++) {
        struct Entity *entity = &entities[i];

        float xmin = 0 - entity->pos_w;
        float xmax = WINDOW_W + entity->pos_w;
        float ymin = 0 - entity->pos_h;
        float ymax = WINDOW_H + entity->pos_h;

        health_kill_if_out_of_range(entity, xmin, xmax, ymin, ymax);
    }
}

struct Entity *health_get_dead_range(struct Entity *entities, int n) {
    for (int i = 0; i < n; i++) {
        struct Entity *entity = &entities[i];

        if (entity->hea_alive == 1) {
            continue;
        }

        return entity;
    }

    return NULL;
}

void render_sync_range(struct Entity *entities, int n) {
    for (int i = 0; i < n; i++) {
        struct Entity *entity = &entities[i];

        if (entity->hea_alive == 0) {
            continue;
        }

        entity->ren_sdl_rect.w = entity->pos_w;
        entity->ren_sdl_rect.h = entity->pos_h;

        entity->ren_sdl_rect.x = floor(entity->pos_pos.x - (entity->pos_w / 2));
        entity->ren_sdl_rect.y = floor(entity->pos_pos.y - (entity->pos_h / 2));
    }
}

void render_update_range(struct Entity *entities, int n, SDL_Renderer *renderer) {
    for (int i = 0; i < n; i++) {
        struct Entity *entity = &entities[i];

        if (entity->hea_alive == 0) {
            continue;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

        int ret = SDL_RenderFillRect(renderer, &entity->ren_sdl_rect);

        if (ret < 0) {
            SDL_Log("ERROR: SDL_RenderFillRect() (%s)", SDL_GetError());
        }
    }
}

int main(void) {
    SDL_Window   *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("ERROR: SDL_Init() (%s)", SDL_GetError());

        return 1;
    }

    window = SDL_CreateWindow("WAT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, 0);

    if (window == NULL) {
        SDL_Log("ERROR: SDL_CreateWindow() (%s)", SDL_GetError());

        SDL_Quit();

        return 1;
    }

    renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        /* SDL_RENDERER_ACCELERATED */
    );

    if (renderer == NULL) {
        SDL_Log("ERROR: SDL_CreateRenderer() (%s)", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    rand_init(&TINYMT_STATE, time(NULL));

    for (int i = 0; i < BULLET_MAX; i++) {
        movement_init(&BULLETS[i], 0, 0, PLAYER_BULLETS_W, PLAYER_BULLETS_H, PLAYER_BULLETS_V);
        health_init(&BULLETS[i], 0);
    }

    /* init players */
    for (int i = 0; i < PLAYER_MAX; i++) {
        movement_init(&PLAYERS[i], WINDOW_W / 2, WINDOW_H / 2, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_V);
        health_init(&PLAYERS[i], 1);

        PLAYERS[i].sho_bullets_n = PLAYER_BULLETS_INIT_N;
        PLAYERS[i].sho_fire_spacing = 128;
    }

    /* init enemies */
    ENEMY_MANAGER.time    = 0.0;
    ENEMY_MANAGER.spacing = 1.0; /* TODO: 100.0 */

    for (int i = 0; i < ENEMY_MAX; i++) {
        movement_init(&ENEMIES[i], 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_V);
        health_init(&ENEMIES[i], 0);
    }

    /* init particles */
    for (int i = 0; i < PARTICLE_MAX; i++) {
        movement_init(&PARTICLES[i], 0, 0, PARTICLE_WIDTH, PARTICLE_HEIGHT, PARTICLE_V);
        health_init(&PARTICLES[i], 0);
    }

    score_init();

    text_init(&HUD_TEXT    , "0"    , TEXT_SIZE_SMALL, TEXT_ALIGN_RIGHT , floor(WINDOW_W - WINDOW_W * 0.05), floor(0 + WINDOW_H * 0.05));
    text_init(&PAUSE_TEXT  , "PAUSE", TEXT_SIZE_LARGE, TEXT_ALIGN_CENTER, floor(WINDOW_W / 2)              , floor(WINDOW_H / 2)       );
    text_init(&WELCOME_TEXT, "WAT"  , TEXT_SIZE_LARGE, TEXT_ALIGN_CENTER, floor(WINDOW_W / 2)              , floor(WINDOW_H / 2)       );

    game_init();

    game_run(renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
