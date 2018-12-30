#include "wat.h"

/* entity */

struct Entity             ENTITIES[ENTITY_MAX];
struct PositionComponent  POSITIONS[POSITION_MAX];
struct MovementComponent  MOVEMENTS[MOVEMENT_MAX];
struct RenderComponent    RENDERS[RENDER_MAX];
struct HealthComponent    HEALTHS[HEALTH_MAX];
struct ShootingComponent  SHOOTINGS[SHOOTING_MAX];

SDL_Rect RENDER_SDL_RECTS[RENDER_SDL_RECT_MAX];
SDL_Rect COLLISION_SDL_RECTS[COLLISION_MAX];

int RENDER_SDL_RECT_IDX[RENDER_MAX];
int COLLISION_SDL_RECT_IDX[COLLISION_MAX];

int ENTITY_POSITION_IDX[ENTITY_MAX];
int ENTITY_MOVEMENT_IDX[ENTITY_MAX];
int ENTITY_RENDER_IDX[ENTITY_MAX];
int ENTITY_HEALTH_IDX[ENTITY_MAX];
int ENTITY_SHOOTING_IDX[ENTITY_MAX];
int ENTITY_COLLISION_IDX[ENTITY_MAX];

int PLAYER_ENTITY_IDX[PLAYER_MAX];
int ENEMY_ENTITY_IDX[ENEMY_MAX];
int BULLET_ENTITY_IDX[BULLET_MAX];
int PARTICLE_ENTITY_IDX[PARTICLE_MAX];

int PLAYER_BULLET_IDX[PLAYER_MAX][BULLET_PER_PLAYER];


/* others */

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
float RUNE_X_POSITIONS[25] = { 0, 1, 2, 3, 4,    0, 1, 2, 3, 4,    0, 1, 2, 3, 4,    0, 1, 2, 3, 4,    0, 1, 2, 3, 4, };
float RUNE_Y_POSITIONS[25] = { 0, 0, 0, 0, 0,    1, 1, 1, 1, 1,    2, 2, 2, 2, 2,    3, 3, 3, 3, 3,    4, 4, 4, 4, 4, };

int RUNES[36][25] = {
    /* 0 */ { 0, 1, 1, 1, 0,    1, 0, 0, 1, 1,    1, 0, 1, 0, 1,    1, 1, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* 1 */ { 0, 0, 1, 0, 0,    1, 1, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    1, 1, 1, 1, 1, },
    /* 2 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 0, 1, 1, 0,    0, 1, 0, 0, 0,    1, 1, 1, 1, 1, },
    /* 3 */ { 1, 1, 1, 1, 0,    0, 0, 0, 0, 1,    0, 0, 1, 1, 0,    0, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* 4 */ { 0, 0, 0, 1, 0,    0, 0, 1, 1, 0,    1, 1, 0, 1, 0,    1, 1, 1, 1, 1,    0, 0, 0, 1, 0, },
    /* 5 */ { 1, 1, 1, 1, 1,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    0, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* 6 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* 7 */ { 1, 1, 1, 1, 1,    0, 0, 0, 0, 1,    0, 0, 0, 1, 0,    0, 0, 1, 0, 0,    0, 1, 0, 0, 0, },
    /* 8 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* 9 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 1,    0, 0, 0, 0, 1,    0, 1, 1, 1, 0, },

    /* A */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1, },
    /* B */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* C */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 0, 0, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* D */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* E */ { 1, 1, 1, 1, 1,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 1, 1, 1, 1, },
    /* F */ { 1, 1, 1, 1, 1,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0, },
    /* G */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 0, 0, 1, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* H */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 1, 1, 1, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1, },
    /* I */ { 1, 1, 1, 1, 1,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    1, 1, 1, 1, 1, },
    /* J */ { 0, 1, 1, 1, 1,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },

    /* K */ { 1, 0, 0, 0, 1,    1, 0, 0, 1, 0,    1, 1, 1, 0, 0,    1, 0, 0, 1, 0,    1, 0, 0, 0, 1, },
    /* L */ { 1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    1, 1, 1, 1, 1, },
    /* M */ { 1, 0, 0, 0, 1,    1, 1, 0, 1, 1,    1, 0, 1, 0, 1,    1, 0, 1, 0, 1,    1, 0, 0, 0, 1, },
    /* N */ { 1, 0, 0, 0, 1,    1, 1, 0, 0, 1,    1, 0, 1, 0, 1,    1, 0, 0, 1, 1,    1, 0, 0, 0, 1, },
    /* O */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* P */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0, },
    /* Q */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 1, 0, 1,    1, 0, 0, 1, 0,    0, 1, 1, 0, 1, },
    /* R */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0,    1, 0, 0, 1, 0,    1, 0, 0, 0, 1, },
    /* S */ { 0, 1, 1, 1, 1,    1, 0, 0, 0, 0,    0, 1, 1, 1, 0,    0, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* T */ { 1, 1, 1, 1, 1,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0, },

    /* U */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* V */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 0, 1, 0,    0, 0, 1, 0, 0, },
    /* W */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 1, 0, 1,    1, 1, 0, 1, 1,    1, 0, 0, 0, 1, },
    /* X */ { 1, 0, 0, 0, 1,    0, 1, 0, 1, 0,    0, 0, 1, 0, 0,    0, 1, 0, 1, 0,    1, 0, 0, 0, 1, },
    /* Y */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 0, 1, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0, },
    /* Z */ { 1, 1, 1, 1, 1,    0, 0, 0, 1, 0,    0, 0, 1, 0, 0,    0, 1, 0, 0, 0,    1, 1, 1, 1, 1, },
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
    int i;

    for (i = 0; i < 25; i++) {
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
    int   i;
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


    for (i = 0; self->value[i] != '\0'; i++) {
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
void player_on_button_a_keydown(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int sid = ENTITY_SHOOTING_IDX[eid];

    SHOOTINGS[sid].firing = 1;
    SHOOTINGS[sid].fire_time = SHOOTINGS[sid].fire_spacing * 1.0;
}

void player_on_button_a_keyup(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int sid = ENTITY_SHOOTING_IDX[eid];

    SHOOTINGS[sid].firing = 0;
    SHOOTINGS[sid].fire_time = 0.0;
}

void player_fire(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pcid = ENTITY_POSITION_IDX[eid];
    int sid = ENTITY_SHOOTING_IDX[eid];

    int i;

    int beid;
    int bpcid;
    int bmcid;
    int bhcid;

    float x;
    float y;

    for (i = 0; i < SHOOTINGS[sid].bullets_n; i++) {
        beid = health_get_dead_range(BULLET_ENTITY_IDX[0], BULLET_ENTITY_IDX[BULLET_MAX - 1]);

        if (beid == -1) {
            return;
        }

        bpcid = ENTITY_POSITION_IDX[beid];
        bmcid = ENTITY_MOVEMENT_IDX[beid];
        bhcid = ENTITY_HEALTH_IDX[beid];

        HEALTHS[bhcid].alive = 1;

        x = POSITIONS[pcid].pos.x + POSITIONS[pcid].w * PLAYER_BULLETS_OFFSET_X[i];
        y = POSITIONS[pcid].pos.y + POSITIONS[pcid].h * PLAYER_BULLETS_OFFSET_Y[i];

        POSITIONS[bpcid].pos.x = x;
        POSITIONS[bpcid].pos.y = y;

        vec2f_set_xy(&MOVEMENTS[bmcid].dir, +0.0, -1.0);
    }
}

void player_fire_update(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int sid = ENTITY_SHOOTING_IDX[eid];

    if (KEYBOARD.z) {
        SHOOTINGS[sid].fire_time += MS_PER_UPDATE; /* TODO: move MS_PER_UPDATE to arguments */
    }

    if (SHOOTINGS[sid].fire_time >= SHOOTINGS[sid].fire_spacing) {
        player_fire(idx);
        SHOOTINGS[sid].fire_time = 0.0;
    }
}

void player_fire_update_all() {
    int i;
    int eid;
    int hcid;

    for (i = 0; i < PLAYER_MAX; i++) {
        eid = PLAYER_ENTITY_IDX[i];
        hcid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 0) {
            continue;
        }

        player_fire_update(i);
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
    int eid = health_get_dead_range(ENEMY_ENTITY_IDX[0], ENEMY_ENTITY_IDX[ENEMY_MAX - 1]);
    int pcid;
    int hcid;
    int mcid;

    if (eid == -1) {
        return;
    }

    pcid = ENTITY_POSITION_IDX[eid];
    hcid = ENTITY_HEALTH_IDX[eid];
    mcid = ENTITY_MOVEMENT_IDX[eid];

    HEALTHS[hcid].alive = 1;

    POSITIONS[pcid].pos.x = rand_n(&TINYMT_STATE, WINDOW_W + 1);
    POSITIONS[pcid].pos.y = 0 - POSITIONS[pcid].h;

    vec2f_set_xy(&MOVEMENTS[mcid].dir, +0.0, +1.0);
}
/* enemy.c end */


/* score.c start */
void score_init() {
    SCORE.value = 0;
}
/* score.c end */


/* collision.c start */
void collision_player_vs_enemies() {
    int peid  = PLAYER_ENTITY_IDX[0];
    int pccid = ENTITY_COLLISION_IDX[peid];
    int phcid = ENTITY_HEALTH_IDX[peid];
    int csrid = COLLISION_SDL_RECT_IDX[pccid];

    int i;

    int eeid;
    int ehcid;
    int eccid;
    int esrid;

    if (HEALTHS[phcid].alive == 0) {
        return;
    }

    for (i = 0; i < ENEMY_MAX; i++) {
        eeid  = ENEMY_ENTITY_IDX[i];
        ehcid = ENTITY_HEALTH_IDX[eeid];
        eccid = ENTITY_COLLISION_IDX[eeid];
        esrid = COLLISION_SDL_RECT_IDX[eccid];

        if (HEALTHS[ehcid].alive == 0) {
            continue;
        }

        if (SDL_HasIntersection(&COLLISION_SDL_RECTS[csrid], &COLLISION_SDL_RECTS[esrid]) == SDL_TRUE) {
            HEALTHS[ehcid].alive = 0;
            continue;
        }
    }
}

void collision_enemies_vs_player_bullets() {
    int i;

    int eeid;
    int epcid;
    int ehcid;
    int eccid;
    int esrid;

    int j;

    int beid;
    int bpcid;
    int bhcid;
    int bsrid;

    for (i = 0; i < ENEMY_MAX; i++) {
        eeid  = ENEMY_ENTITY_IDX[i];
        epcid = ENTITY_POSITION_IDX[eeid];
        ehcid = ENTITY_HEALTH_IDX[eeid];
        eccid = ENTITY_COLLISION_IDX[eeid];
        esrid = COLLISION_SDL_RECT_IDX[eccid];

        if (HEALTHS[ehcid].alive == 0) {
            continue;
        }

        for (j = 0; j < BULLET_PER_PLAYER; j++) {
            beid  = BULLET_ENTITY_IDX[j];
            bpcid = ENTITY_POSITION_IDX[beid];
            bhcid = ENTITY_HEALTH_IDX[beid];
            bsrid = COLLISION_SDL_RECT_IDX[bpcid];

            if (HEALTHS[bhcid].alive == 0) {
                continue;
            }

            if (SDL_HasIntersection(&COLLISION_SDL_RECTS[esrid], &COLLISION_SDL_RECTS[bsrid]) == SDL_TRUE) {
                HEALTHS[ehcid].alive = 0;
                HEALTHS[bhcid].alive = 0;

                collision_make_explosion(POSITIONS[epcid].pos.x, POSITIONS[epcid].pos.y);

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
    int i;

    int eid;
    int pcid;
    int hcid;
    int mcid;

    for (i = 0; i < EXPLOSION_PARTICLES_N; i++) {
        eid = health_get_dead_range(PARTICLE_ENTITY_IDX[0], PARTICLE_ENTITY_IDX[PARTICLE_MAX - 1]);

        if (eid == -1) {
            return;
        }

        pcid = ENTITY_POSITION_IDX[eid];
        mcid = ENTITY_MOVEMENT_IDX[eid];
        hcid = ENTITY_HEALTH_IDX[eid];

        POSITIONS[pcid].pos.x = x;
        POSITIONS[pcid].pos.y = y;

        MOVEMENTS[mcid].dir = EXPLOSION_PARTICLES_DIR[i];

        HEALTHS[hcid].alive = 1;
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
    int peid  = PLAYER_ENTITY_IDX[0];
    int pmcid = ENTITY_MOVEMENT_IDX[peid];

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

    vec2f_set_xy(&MOVEMENTS[pmcid].dir, tmp_x, tmp_y);

    movement_update_range(0, ENTITY_MAX - 1);

    player_fire_update_all();

    movement_fclamp_player(peid);

    health_kill_if_out_of_map_range(BULLET_ENTITY_IDX[0], BULLET_ENTITY_IDX[BULLET_MAX - 1]);
    health_kill_if_out_of_map_range(ENEMY_ENTITY_IDX[0], ENEMY_ENTITY_IDX[ENEMY_MAX - 1]);
    health_kill_if_out_of_map_range(PARTICLE_ENTITY_IDX[0], PARTICLE_ENTITY_IDX[BULLET_MAX - 1]);

    collision_sync_range(PLAYER_ENTITY_IDX[0], PLAYER_ENTITY_IDX[PLAYER_MAX - 1]);
    collision_sync_range(ENEMY_ENTITY_IDX[0], ENEMY_ENTITY_IDX[ENEMY_MAX - 1]);
    collision_sync_range(BULLET_ENTITY_IDX[0], BULLET_ENTITY_IDX[BULLET_MAX - 1]);

    collision_update();

    enemy_try_spawn();

    hud_update();
}

void in_game_state_render(SDL_Renderer *renderer) {
    render_sync_range(PARTICLE_ENTITY_IDX[0], PARTICLE_ENTITY_IDX[PARTICLE_MAX - 1]);
    render_sync_range(PLAYER_ENTITY_IDX[0], PLAYER_ENTITY_IDX[PLAYER_MAX - 1]);
    render_sync_range(ENEMY_ENTITY_IDX[0], ENEMY_ENTITY_IDX[ENEMY_MAX - 1]);
    render_sync_range(BULLET_ENTITY_IDX[0], BULLET_ENTITY_IDX[BULLET_MAX - 1]);

    render_update_range(PLAYER_ENTITY_IDX[0], PLAYER_ENTITY_IDX[PLAYER_MAX - 1], renderer);
    render_update_range(ENEMY_ENTITY_IDX[0], ENEMY_ENTITY_IDX[ENEMY_MAX - 1], renderer);
    render_update_range(BULLET_ENTITY_IDX[0], BULLET_ENTITY_IDX[BULLET_MAX - 1], renderer);
    render_update_range(PARTICLE_ENTITY_IDX[0], PARTICLE_ENTITY_IDX[PARTICLE_MAX - 1], renderer);

    hud_render(renderer);
}
/* in_game_state.c end */


/* pause_state.c start */
void pause_state_update() {

}

void pause_state_render(SDL_Renderer *renderer) {
    render_sync_range(PARTICLE_ENTITY_IDX[0], PARTICLE_ENTITY_IDX[PARTICLE_MAX - 1]);
    render_sync_range(PLAYER_ENTITY_IDX[0], PLAYER_ENTITY_IDX[PLAYER_MAX - 1]);
    render_sync_range(ENEMY_ENTITY_IDX[0], ENEMY_ENTITY_IDX[ENEMY_MAX - 1]);
    render_sync_range(BULLET_ENTITY_IDX[0], BULLET_ENTITY_IDX[BULLET_MAX - 1]);

    render_update_range(PLAYER_ENTITY_IDX[0], PLAYER_ENTITY_IDX[PLAYER_MAX - 1], renderer);
    render_update_range(ENEMY_ENTITY_IDX[0], ENEMY_ENTITY_IDX[ENEMY_MAX - 1], renderer);
    render_update_range(BULLET_ENTITY_IDX[0], BULLET_ENTITY_IDX[BULLET_MAX - 1], renderer);
    render_update_range(PARTICLE_ENTITY_IDX[0], PARTICLE_ENTITY_IDX[PARTICLE_MAX - 1], renderer);

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

        SDL_RenderPresent(renderer);

        /* ----- */
        debug_end = SDL_GetPerformanceCounter();
        debug_elapsed = performance_counters_to_ms(debug_start, debug_end);
        /* SDL_Log("frame time: %f ms", debug_elapsed); */
        /* ----- */

        /* SDL_Delay(SLEEP_MS); */
    }
}
/* game.c end */


/* input.c start */
void input_update() {
    int sym;

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
                player_on_button_a_keydown(0);
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
                player_on_button_a_keyup(0);
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


void movement_init(int eid, float x, float y, int w, int h, int v) {
    int pcid  = ENTITY_POSITION_IDX[eid];
    int mcid  = ENTITY_MOVEMENT_IDX[eid];

    POSITIONS[pcid].w = w;
    POSITIONS[pcid].h = h;

    POSITIONS[pcid].pos.x = x;
    POSITIONS[pcid].pos.y = y;

    MOVEMENTS[mcid].vel = v;
}

void movement_fclamp_player(int eid) {
    int   pcid;
    float x_min;
    float x_max;
    float y_min;
    float y_max;

    pcid  = ENTITY_POSITION_IDX[eid];

    x_min = 0 + POSITIONS[pcid].w / 2;
    x_max = WINDOW_W - POSITIONS[pcid].w / 2;
    y_min = 0 + POSITIONS[pcid].h / 2;
    y_max = WINDOW_H - POSITIONS[pcid].h / 2;

    POSITIONS[pcid].pos.x = fclamp(POSITIONS[pcid].pos.x, x_min, x_max);
    POSITIONS[pcid].pos.y = fclamp(POSITIONS[pcid].pos.y, y_min, y_max);

}

void movement_update(int idx) {
    int pcid = ENTITY_POSITION_IDX[idx];
    int mcid = ENTITY_MOVEMENT_IDX[idx];
    
    if (MOVEMENTS[pcid].dir.x != 0.0 || MOVEMENTS[pcid].dir.y != 0.0) {
        vec2f_normalize(&MOVEMENTS[mcid].dir);
        
        POSITIONS[pcid].pos.x += (MOVEMENTS[mcid].vel * MOVEMENTS[mcid].dir.x) / UPDATES_PER_SECOND;
        POSITIONS[pcid].pos.y += (MOVEMENTS[mcid].vel * MOVEMENTS[mcid].dir.y) / UPDATES_PER_SECOND;
    }
}

void movement_update_range(int start, int end) {
    int eid;
    int hcid;

    for (eid = start; eid <= end; eid++) {
        hcid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 0) {
            continue;
        }

        movement_update(eid);
    }
}

void collision_sync_range(int start, int end) {
    int eid;
    int hcid;
    int ccid;
    int pcid;
    int csrid;

    for (eid = start; eid <= end; eid++) {
        hcid  = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 0) {
            continue;
        }

        ccid  = ENTITY_COLLISION_IDX[eid];
        pcid  = ENTITY_POSITION_IDX[eid];
        csrid = COLLISION_SDL_RECT_IDX[ccid];

        COLLISION_SDL_RECTS[csrid].w = POSITIONS[pcid].w;
        COLLISION_SDL_RECTS[csrid].h = POSITIONS[pcid].h;

        COLLISION_SDL_RECTS[csrid].x = floor(POSITIONS[pcid].pos.x - (POSITIONS[pcid].w / 2));
        COLLISION_SDL_RECTS[csrid].y = floor(POSITIONS[pcid].pos.y - (POSITIONS[pcid].h / 2));
    }
}

void health_init(int eid, int alive) {
    int hcid  = ENTITY_HEALTH_IDX[eid];

    HEALTHS[hcid].alive = alive;
}

void health_kill_if_out_of_range(int eid, float xmin, float xmax, float ymin, float ymax) {
    int pcid  = ENTITY_POSITION_IDX[eid];
    int hcid  = ENTITY_HEALTH_IDX[eid];

    if (POSITIONS[pcid].pos.x < xmin || POSITIONS[pcid].pos.x > xmax) {
        HEALTHS[hcid].alive = 0;

        POSITIONS[pcid].pos.x = 0;
        POSITIONS[pcid].pos.y = 0;

        return;
    }

    if (POSITIONS[pcid].pos.y < ymin || POSITIONS[pcid].pos.y > ymax) {
        HEALTHS[hcid].alive = 0;

        POSITIONS[pcid].pos.x = 0;
        POSITIONS[pcid].pos.y = 0;

        return;
    }
}

void health_kill_if_out_of_map_range(int start, int end) {
    int   eid;
    int   pcid;
    float xmin;
    float xmax;
    float ymin;
    float ymax;

    for (eid = start; eid <= end; eid++) {
        pcid  = ENTITY_POSITION_IDX[eid];

        xmin = 0 - POSITIONS[pcid].w;
        xmax = WINDOW_W + POSITIONS[pcid].w;
        ymin = 0 - POSITIONS[pcid].h;
        ymax = WINDOW_H + POSITIONS[pcid].h;

        health_kill_if_out_of_range(eid, xmin, xmax, ymin, ymax);
    }
}

int health_get_dead_range(int start, int end) {
    int eid;
    int hcid;

    for (eid = start; eid <= end; eid++) {
        hcid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 1) {
            continue;
        }

        return eid;
    }

    return -1;
}

void render_sync_range(int start, int end) {
    int eid;
    int hcid;
    int rcid;
    int pcid;
    int rsrid;

    for (eid = start; eid <= end; eid++) {
        hcid  = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 0) {
            continue;
        }

        rcid  = ENTITY_RENDER_IDX[eid];
        pcid  = ENTITY_POSITION_IDX[eid];
        rsrid = RENDER_SDL_RECT_IDX[rcid];

        RENDER_SDL_RECTS[rsrid].w = POSITIONS[pcid].w;
        RENDER_SDL_RECTS[rsrid].h = POSITIONS[pcid].h;

        RENDER_SDL_RECTS[rsrid].x = floor(POSITIONS[pcid].pos.x - (POSITIONS[pcid].w / 2));
        RENDER_SDL_RECTS[rsrid].y = floor(POSITIONS[pcid].pos.y - (POSITIONS[pcid].h / 2));
    }
}

void render_update_range(int start, int end, SDL_Renderer *renderer) {
    int eid;
    int ret = 0;
    int hcid;
    int rcid;
    int srid;

    for (eid = start; eid <= end; eid++) {
        hcid = ENTITY_HEALTH_IDX[eid];
        rcid = ENTITY_RENDER_IDX[eid];
        srid = RENDER_SDL_RECT_IDX[rcid];

        if (HEALTHS[hcid].alive == 0) {
            continue;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

        ret = SDL_RenderFillRect(renderer, &RENDER_SDL_RECTS[srid]);

        if (ret < 0) {
            SDL_Log("ERROR: SDL_RenderFillRect() (%s)", SDL_GetError());
        }
    }
}


void render_sdl_rect_idx_set_up() {
    int i;
    int render_sdl_rect_i = 0;

    for (i = 0; i < RENDER_MAX; i++) {
        RENDER_SDL_RECT_IDX[i] = render_sdl_rect_i;

        render_sdl_rect_i++;
    }
}

void collision_sdl_rect_idx_set_up() {
    int i;
    int collision_sdl_rect_i = 0;

    for (i = 0; i < COLLISION_MAX; i++) {
        COLLISION_SDL_RECT_IDX[i] = collision_sdl_rect_i;

        collision_sdl_rect_i++;
    }
}

void entity_set_up() {
    int i;
    int j;
    int entity_i    = 0;
    int position_i  = 0;
    int movement_i  = 0;
    int render_i    = 0;
    int health_i    = 0;
    int shooting_i  = 0;
    int collision_i = 0;
    int bullet_i    = 0;

    render_sdl_rect_idx_set_up();
    collision_sdl_rect_idx_set_up();

    for (i = 0; i < PLAYER_MAX; i++) {
        ENTITIES[entity_i].type = ENTITY_TYPE_PLAYER;

        PLAYER_ENTITY_IDX[i] = entity_i;

        ENTITY_POSITION_IDX[entity_i]  = position_i;
        ENTITY_MOVEMENT_IDX[entity_i]  = movement_i;
        ENTITY_RENDER_IDX[entity_i]    = render_i;
        ENTITY_HEALTH_IDX[entity_i]    = health_i;
        ENTITY_SHOOTING_IDX[entity_i]  = shooting_i;
        ENTITY_COLLISION_IDX[entity_i] = collision_i;
        
        entity_i++;
        position_i++;
        movement_i++;
        render_i++;
        health_i++;
        shooting_i++;
        collision_i++;
    }

    for (i = 0; i < ENEMY_MAX; i++) {
        ENTITIES[entity_i].type = ENTITY_TYPE_ENEMY;

        ENEMY_ENTITY_IDX[i] = entity_i;

        ENTITY_POSITION_IDX[entity_i]  = position_i;
        ENTITY_MOVEMENT_IDX[entity_i]  = movement_i;
        ENTITY_RENDER_IDX[entity_i]    = render_i;
        ENTITY_HEALTH_IDX[entity_i]    = health_i;
        ENTITY_SHOOTING_IDX[entity_i]  = shooting_i;
        ENTITY_COLLISION_IDX[entity_i] = collision_i;

        entity_i++;
        position_i++;
        movement_i++;
        render_i++;
        health_i++;
        shooting_i++;
        collision_i++;
    }

    for (i = 0; i < BULLET_MAX; i++) {
        ENTITIES[entity_i].type = ENTITY_TYPE_BULLET;

        BULLET_ENTITY_IDX[i] = entity_i;

        ENTITY_POSITION_IDX[entity_i]  = position_i;
        ENTITY_MOVEMENT_IDX[entity_i]  = movement_i;
        ENTITY_RENDER_IDX[entity_i]    = render_i;
        ENTITY_HEALTH_IDX[entity_i]    = health_i;
        ENTITY_SHOOTING_IDX[entity_i]  = -1;
        ENTITY_COLLISION_IDX[entity_i] = collision_i;

        entity_i++;
        position_i++;
        movement_i++;
        render_i++;
        health_i++;
        collision_i++;
    }

    bullet_i = 0;

    for (i = 0; i < PLAYER_MAX; i++) {
        for (j = 0; j < BULLET_PER_PLAYER; j++) {
            PLAYER_BULLET_IDX[i][j] = bullet_i;

            bullet_i++;
        }
    }

    for (i = 0; i < PARTICLE_MAX; i++) {
        ENTITIES[entity_i].type = ENTITY_TYPE_PARTICLE;

        PARTICLE_ENTITY_IDX[i] = entity_i;

        ENTITY_POSITION_IDX[entity_i]  = position_i;
        ENTITY_MOVEMENT_IDX[entity_i]  = movement_i;
        ENTITY_RENDER_IDX[entity_i]    = render_i;
        ENTITY_HEALTH_IDX[entity_i]    = health_i;
        ENTITY_SHOOTING_IDX[entity_i]  = -1;
        ENTITY_COLLISION_IDX[entity_i] = -1;

        entity_i++;
        position_i++;
        movement_i++;
        render_i++;
        health_i++;
    }
}


int main(void) {
    SDL_Window   *window;
    SDL_Renderer *renderer;

    int i;
    int eid;
    int sid;

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
        /* SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC */
        SDL_RENDERER_ACCELERATED
    );

    if (renderer == NULL) {
        SDL_Log("ERROR: SDL_CreateRenderer() (%s)", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    entity_set_up();

    rand_init(&TINYMT_STATE, time(NULL));

    for (i = 0; i < BULLET_MAX; i++) {
        eid = BULLET_ENTITY_IDX[i];

        movement_init(eid, 0, 0, PLAYER_BULLETS_W, PLAYER_BULLETS_H, PLAYER_BULLETS_V);
        health_init(eid, 0);
    }

    /* init players */
    for (i = 0; i < PLAYER_MAX; i++) {
        eid = PLAYER_ENTITY_IDX[i];
        sid = ENTITY_SHOOTING_IDX[eid];

        movement_init(eid, WINDOW_W / 2, WINDOW_H / 2, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_V);
        health_init(eid, 1);

        SHOOTINGS[sid].bullets_n = PLAYER_BULLETS_INIT_N;
        SHOOTINGS[sid].fire_spacing = 128;
    }

    /* init enemies */
    ENEMY_MANAGER.time    = 0.0;
    ENEMY_MANAGER.spacing = 100.0; /* TODO: 100.0 */

    for (i = 0; i < ENEMY_MAX; i++) {
        eid = ENEMY_ENTITY_IDX[i];

        movement_init(eid, 0, 0, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_V);
        health_init(eid, 0);
    }

    /* init particles */
    for (i = 0; i < PARTICLE_MAX; i++) {
        eid = PARTICLE_ENTITY_IDX[i];

        movement_init(eid, 0, 0, PARTICLE_WIDTH, PARTICLE_HEIGHT, PARTICLE_V);
        health_init(eid, 0);
    }

    score_init();

    text_init(
        &HUD_TEXT,
        "0",
        TEXT_SIZE_SMALL,
        TEXT_ALIGN_RIGHT,
        floor(WINDOW_W - WINDOW_W * 0.05),
        floor(0 + WINDOW_H * 0.05)
    );

    text_init(
        &PAUSE_TEXT,
        "PAUSE",
        TEXT_SIZE_LARGE,
        TEXT_ALIGN_CENTER,
        floor(WINDOW_W / 2),
        floor(WINDOW_H / 2)
    );

    text_init(
        &WELCOME_TEXT,
        "WAT",
        TEXT_SIZE_LARGE,
        TEXT_ALIGN_CENTER,
        floor(WINDOW_W / 2),
        floor(WINDOW_H / 2)
    );

    game_init();

    game_run(renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
