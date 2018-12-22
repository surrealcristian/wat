#include "wat.h"

// entity

struct PositionComponent  POSITIONS[POSITION_MAX];
struct MovementComponent  MOVEMENTS[MOVEMENT_MAX];
struct RenderComponent    RENDERS[RENDER_MAX];
struct HealthComponent    HEALTHS[HEALTH_MAX];
struct ShootingComponent  SHOOTINGS[SHOOTING_MAX];
struct CollisionComponent COLLISIONS[COLLISION_MAX];

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


// others

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



// util.c start
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
// util.c end


// text.c start
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
    float rune_x          = 0;
    float rune_y          = 0;

    if (self->align == TEXT_ALIGN_LEFT) {
        rune_x = self->x;
    } else if (self->align == TEXT_ALIGN_CENTER) {
        rune_x = self->x - (text_w_px / 2);
    } else if (self->align == TEXT_ALIGN_RIGHT) {
        rune_x = self->x - text_w_px;
    }

    rune_y = self->y - (rune_h_px / 2);

    int idx;

    for (int i = 0; self->value[i] != '\0'; i++) {
        int c = self->value[i];

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
// text.c end


// config.c start
float PLAYER_BULLETS_OFFSET_X[5] = { +0.00, -0.75, +0.75, -1.50, +1.50 };
float PLAYER_BULLETS_OFFSET_Y[5] = { -1.00, -0.50, -0.50, +0.00, +0.00 };

float EXPLOSION_PARTICLES_VX[4] = { -1.00, +1.00, -1.00, +1.00 };
float EXPLOSION_PARTICLES_VY[4] = { -1.00, -1.00, +1.00, +1.00 };
// config.c end


// bullet.c start
void bullet_init_all(int w, int h, int v) {
    for (int i = 0; i < BULLET_MAX; i++) {
        int eid = BULLET_ENTITY_IDX[i];

        movement_init(eid, 0, 0, w, h, v);
        health_init(eid, 0);
    }
}
// bullet.c end


// player.c start
void player_init(int idx, float x, float y, int w, int h, int v) {
    int eid   = PLAYER_ENTITY_IDX[idx];
    int pcid  = ENTITY_POSITION_IDX[eid];
    int mcid  = ENTITY_MOVEMENT_IDX[eid];
    int hcid  = ENTITY_HEALTH_IDX[eid];
    int sid   = ENTITY_SHOOTING_IDX[eid];

    POSITIONS[pcid].w = w;
    POSITIONS[pcid].h = h;

    player_set_x(idx, x);
    player_set_y(idx, y);

    MOVEMENTS[mcid].v = v;

    HEALTHS[hcid].alive = 1;

    SHOOTINGS[sid].bullets_n = PLAYER_BULLETS_INIT_N;
    SHOOTINGS[sid].fire_spacing = 128;
}

void player_set_x(int idx, float value) {
    int eid   = PLAYER_ENTITY_IDX[idx];
    int pcid  = ENTITY_POSITION_IDX[eid];

    float x_min = 0 + POSITIONS[pcid].w / 2;
    float x_max = WINDOW_W - POSITIONS[pcid].w / 2;

    if (value < x_min) {
        value = x_min;
    } else if (value > x_max) {
        value = x_max;
    }

    POSITIONS[pcid].x = value;
}

void player_set_y(int idx, float value) {
    int eid   = PLAYER_ENTITY_IDX[idx];
    int pcid  = ENTITY_POSITION_IDX[eid];

    float y_min = 0 + POSITIONS[pcid].h / 2;
    float y_max = WINDOW_H - POSITIONS[pcid].h / 2;

    if (value < y_min) {
        value = y_min;
    } else if (value > y_max) {
        value = y_max;
    }

    POSITIONS[pcid].y = value;
}

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

    for (int i = 0; i < SHOOTINGS[sid].bullets_n; i++) {
        int beid = health_get_dead_range(BULLET_ENTITY_IDX[0], BULLET_ENTITY_IDX[BULLET_MAX - 1]);

        if (beid == -1) {
            return;
        }

        int bmcid = ENTITY_MOVEMENT_IDX[beid];
        int bhcid = ENTITY_HEALTH_IDX[beid];

        HEALTHS[bhcid].alive = 1;

        float x = POSITIONS[pcid].x + POSITIONS[pcid].w * PLAYER_BULLETS_OFFSET_X[i];
        float y = POSITIONS[pcid].y + POSITIONS[pcid].h * PLAYER_BULLETS_OFFSET_Y[i];

        entity_set_x(beid, x);
        entity_set_y(beid, y);

        MOVEMENTS[bmcid].vx = PLAYER_BULLETS_VX;
        MOVEMENTS[bmcid].vy = PLAYER_BULLETS_VY;
    }
}

void player_update(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pcid = ENTITY_POSITION_IDX[eid];
    int mcid = ENTITY_MOVEMENT_IDX[eid];

    MOVEMENTS[mcid].vx = 0;
    MOVEMENTS[mcid].vy = 0;

    if (KEYBOARD.right) {
        MOVEMENTS[mcid].vx = +1;
    }

    if (KEYBOARD.left) {
        MOVEMENTS[mcid].vx = -1;
    }

    if (KEYBOARD.up) {
        MOVEMENTS[mcid].vy = -1;
    }

    if (KEYBOARD.down) {
        MOVEMENTS[mcid].vy = +1;
    }

    float x = POSITIONS[pcid].x + (1.0 * MOVEMENTS[mcid].v * MOVEMENTS[mcid].vx / UPDATES_PER_SECOND);
    float y = POSITIONS[pcid].y + (1.0 * MOVEMENTS[mcid].v * MOVEMENTS[mcid].vy / UPDATES_PER_SECOND);

    player_set_x(idx, x);
    player_set_y(idx, y);
}

void player_fire_update(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int sid = ENTITY_SHOOTING_IDX[eid];

    if (KEYBOARD.z) {
        SHOOTINGS[sid].fire_time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments
    }

    if (SHOOTINGS[sid].fire_time >= SHOOTINGS[sid].fire_spacing) {
        player_fire(idx);
        SHOOTINGS[sid].fire_time = 0.0;
    }
}

void player_update_all() {
    for (int i = 0; i < PLAYER_MAX; i++) {
        int eid = PLAYER_ENTITY_IDX[i];
        int hcid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 0) {
            continue;
        }

        player_update(i);
    }
}

void player_fire_update_all() {
    for (int i = 0; i < PLAYER_MAX; i++) {
        int eid = PLAYER_ENTITY_IDX[i];
        int hcid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 0) {
            continue;
        }

        player_fire_update(i);
    }
}
// player.c end


// enemy.c start
void enemy_init_all(int w, int h, int v) {
    ENEMY_MANAGER.time    = 0.0;
    ENEMY_MANAGER.spacing = 100.0;

    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eid = ENEMY_ENTITY_IDX[i];

        movement_init(eid, 0, 0, w, h, v);
        health_init(eid, 0);
    }
}

void enemy_try_spawn() {
    ENEMY_MANAGER.time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments

    if (ENEMY_MANAGER.time >= ENEMY_MANAGER.spacing) {
        enemy_spawn();
        ENEMY_MANAGER.time = 0.0;
    }
}

void enemy_spawn() {
    int eid = health_get_dead_range(ENEMY_ENTITY_IDX[0], ENEMY_ENTITY_IDX[ENEMY_MAX - 1]);

    if (eid == -1) {
        return;
    }

    int pcid = ENTITY_POSITION_IDX[eid];
    int hcid = ENTITY_HEALTH_IDX[eid];
    int mcid = ENTITY_MOVEMENT_IDX[eid];

    HEALTHS[hcid].alive = 1;

    entity_set_x(eid, rand_n(&TINYMT_STATE, WINDOW_W + 1));
    entity_set_y(eid, 0 - POSITIONS[pcid].h);

    MOVEMENTS[mcid].vx = ENEMY_VX;
    MOVEMENTS[mcid].vy = ENEMY_VY;
}
// enemy.c end


// particle.c start
void particle_init(int i, float x, float y, int w, int h, int v) {
    int eid   = PARTICLE_ENTITY_IDX[i];
    int pcid  = ENTITY_POSITION_IDX[eid];
    int mcid  = ENTITY_MOVEMENT_IDX[eid];
    int rcid  = ENTITY_RENDER_IDX[eid];
    int hcid  = ENTITY_HEALTH_IDX[eid];
    int ccid  = ENTITY_COLLISION_IDX[eid];
    int csrid = COLLISION_SDL_RECT_IDX[ccid];
    int rsrid = RENDER_SDL_RECT_IDX[rcid];

    POSITIONS[pcid].w = w;
    POSITIONS[pcid].h = h;

    COLLISION_SDL_RECTS[csrid].w = POSITIONS[pcid].w;
    COLLISION_SDL_RECTS[csrid].h = POSITIONS[pcid].h;

    RENDER_SDL_RECTS[rsrid].w = POSITIONS[pcid].w;
    RENDER_SDL_RECTS[rsrid].h = POSITIONS[pcid].h;

    entity_set_x(eid, x);
    entity_set_y(eid, y);

    MOVEMENTS[mcid].v = v;

    HEALTHS[hcid].alive = 0;
}

void particle_init_all(int w, int h, int v) {
    for (int i = 0; i < PARTICLE_MAX; i++) {
        particle_init(i, 0, 0, w, h, v);
    }
}
// particle.c end


// score.c start
void score_init() {
    SCORE.value = 0;
}
// score.c end


// collision.c start
void collision_player_vs_enemies() {
    int peid  = PLAYER_ENTITY_IDX[0];
    int pccid = ENTITY_COLLISION_IDX[peid];
    int phcid = ENTITY_HEALTH_IDX[peid];
    int csrid = COLLISION_SDL_RECT_IDX[pccid];

    if (HEALTHS[phcid].alive == 0) {
        return;
    }

    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eeid  = ENEMY_ENTITY_IDX[i];
        int ehcid = ENTITY_HEALTH_IDX[eeid];
        int eccid = ENTITY_COLLISION_IDX[eeid];
        int esrid = COLLISION_SDL_RECT_IDX[eccid];

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
    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eeid  = ENEMY_ENTITY_IDX[i];
        int epcid = ENTITY_POSITION_IDX[eeid];
        int ehcid = ENTITY_HEALTH_IDX[eeid];
        int eccid = ENTITY_COLLISION_IDX[eeid];
        int esrid = COLLISION_SDL_RECT_IDX[eccid];

        if (HEALTHS[ehcid].alive == 0) {
            continue;
        }

        for (int j = 0; j < BULLET_PER_PLAYER; j++) {
            int beid  = BULLET_ENTITY_IDX[j];
            int bpcid = ENTITY_POSITION_IDX[beid];
            int bhcid = ENTITY_HEALTH_IDX[beid];
            int bsrid = COLLISION_SDL_RECT_IDX[bpcid];

            if (HEALTHS[bhcid].alive == 0) {
                continue;
            }

            if (SDL_HasIntersection(&COLLISION_SDL_RECTS[esrid], &COLLISION_SDL_RECTS[bsrid]) == SDL_TRUE) {
                HEALTHS[ehcid].alive = 0;
                HEALTHS[bhcid].alive = 0;

                collision_make_explosion(POSITIONS[epcid].x, POSITIONS[epcid].y);

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
        int eid = health_get_dead_range(PARTICLE_ENTITY_IDX[0], PARTICLE_ENTITY_IDX[PARTICLE_MAX - 1]);

        if (eid == -1) {
            return;
        }

        int hcid = ENTITY_HEALTH_IDX[eid];
        int mcid = ENTITY_MOVEMENT_IDX[eid];

        entity_set_x(eid, x);
        entity_set_y(eid, y);

        MOVEMENTS[mcid].vx = EXPLOSION_PARTICLES_VX[i];
        MOVEMENTS[mcid].vy = EXPLOSION_PARTICLES_VY[i];

        HEALTHS[hcid].alive = 1;
    }
}
// collision.c end


// welcome_state.c start
void welcome_state_update() {

}

void welcome_state_render(SDL_Renderer *renderer) {
    text_render(&WELCOME_TEXT, renderer);
}
// welcome_state.c end


// hud.c start
char HUD_SCORE_TEXT[HUD_SCORE_TEXT_BUFSIZE];

void hud_update() {
    snprintf(HUD_SCORE_TEXT, HUD_SCORE_TEXT_BUFSIZE, "%lu", SCORE.value);

    text_set_value(&HUD_TEXT, HUD_SCORE_TEXT);
}

void hud_render(SDL_Renderer *renderer) {
    text_render(&HUD_TEXT, renderer);
}
// hud.c end


// in_game_state.c start
void in_game_state_update() {
    player_update_all();

    movement_update_range(BULLET_ENTITY_IDX[0], BULLET_ENTITY_IDX[BULLET_MAX - 1]);
    movement_update_range(ENEMY_ENTITY_IDX[0], ENEMY_ENTITY_IDX[ENEMY_MAX - 1]);
    movement_update_range(PARTICLE_ENTITY_IDX[0], PARTICLE_ENTITY_IDX[PARTICLE_MAX - 1]);

    player_fire_update_all();

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
// in_game_state.c end


// pause_state.c start
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
// pause_state.c end


// game.c start
void game_init() {
    GAME.state = STATE_WELCOME;
}

void game_run(SDL_Renderer *renderer) {
    int keep_running = 1;

    Uint64 previous = SDL_GetPerformanceCounter();
    double lag = 0.0;

    while (keep_running) {
        // -----
        Uint64 debug_start = SDL_GetPerformanceCounter();
        // -----

        Uint64 current = SDL_GetPerformanceCounter();
        double elapsed = performance_counters_to_ms(previous, current);

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

        // RENDER

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // grey
        SDL_RenderClear(renderer);

        if (GAME.state == STATE_WELCOME) {
            welcome_state_render(renderer);
        } else if (GAME.state == STATE_IN_GAME) {
            in_game_state_render(renderer);
        } else if (GAME.state == STATE_PAUSE) {
            pause_state_render(renderer);
        }

        SDL_RenderPresent(renderer);

        // -----
        Uint64 debug_end = SDL_GetPerformanceCounter();
        double debug_elapsed = performance_counters_to_ms(debug_start, debug_end);
        //SDL_Log("frame time: %f ms", debug_elapsed); //TODO
        // -----

        //SDL_Delay(SLEEP_MS);
    }
}
// game.c end


// input.c start
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
// input.c end



void entity_set_x(int idx, float value) {
    int pcid  = ENTITY_POSITION_IDX[idx];
    int hcid  = ENTITY_HEALTH_IDX[idx];

    float x_min = 0 - POSITIONS[pcid].w - 1;
    float x_max = WINDOW_W + POSITIONS[pcid].w + 1;

    if (value < x_min || value > x_max) {
        HEALTHS[hcid].alive = 0;

        value = 0;
        POSITIONS[pcid].x = 0;
        POSITIONS[pcid].y = 0;

        return;
    }

    POSITIONS[pcid].x = value;
}

void entity_set_y(int idx, float value) {
    int pcid  = ENTITY_POSITION_IDX[idx];
    int hcid  = ENTITY_HEALTH_IDX[idx];

    float y_min = 0 - POSITIONS[pcid].h - 1;
    float y_max = WINDOW_H + POSITIONS[pcid].h + 1;

    if (value < y_min || value > y_max) {
        HEALTHS[hcid].alive = 0;

        value = 0;
        POSITIONS[pcid].x = 0;
        POSITIONS[pcid].y = 0;

        return;
    }

    POSITIONS[pcid].y = value;
}

void movement_init(int eid, float x, float y, int w, int h, int v) {
    int pcid  = ENTITY_POSITION_IDX[eid];
    int mcid  = ENTITY_MOVEMENT_IDX[eid];

    POSITIONS[pcid].w = w;
    POSITIONS[pcid].h = h;

    entity_set_x(eid, x);
    entity_set_y(eid, y);

    MOVEMENTS[mcid].v = v;
}

void movement_update(int idx) {
    int pcid = ENTITY_POSITION_IDX[idx];
    int mcid = ENTITY_MOVEMENT_IDX[idx];

    float x = POSITIONS[pcid].x + (1.0 * MOVEMENTS[pcid].v * MOVEMENTS[mcid].vx / UPDATES_PER_SECOND);
    float y = POSITIONS[pcid].y + (1.0 * MOVEMENTS[pcid].v * MOVEMENTS[mcid].vy / UPDATES_PER_SECOND);

    entity_set_x(idx, x);
    entity_set_y(idx, y);
}

void movement_update_range(int start, int end) {
    for (int eid = start; eid <= end; eid++) {
        int hcid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 0) {
            continue;
        }

        movement_update(eid);
    }
}

void collision_sync_range(int start, int end) {
    for (int eid = start; eid <= end; eid++) {
        int hcid  = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 0) {
            continue;
        }

        int ccid  = ENTITY_COLLISION_IDX[eid];
        int pcid  = ENTITY_POSITION_IDX[eid];
        int csrid = COLLISION_SDL_RECT_IDX[ccid];

        COLLISION_SDL_RECTS[csrid].w = POSITIONS[pcid].w;
        COLLISION_SDL_RECTS[csrid].h = POSITIONS[pcid].h;

        COLLISION_SDL_RECTS[csrid].x = floor(POSITIONS[pcid].x - (POSITIONS[pcid].w / 2));
        COLLISION_SDL_RECTS[csrid].y = floor(POSITIONS[pcid].y - (POSITIONS[pcid].h / 2));
    }
};

void health_init(int eid, int alive) {
    int hcid  = ENTITY_HEALTH_IDX[eid];

    HEALTHS[hcid].alive = alive;
}

int health_get_dead_range(int start, int end) {
    for (int eid = start; eid <= end; eid++) {
        int hcid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 1) {
            continue;
        }

        return eid;
    }

    return -1;
}

void render_sync_range(int start, int end) {
    for (int eid = start; eid <= end; eid++) {
        int hcid  = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hcid].alive == 0) {
            continue;
        }

        int rcid  = ENTITY_RENDER_IDX[eid];
        int pcid  = ENTITY_POSITION_IDX[eid];
        int rsrid = RENDER_SDL_RECT_IDX[rcid];

        RENDER_SDL_RECTS[rsrid].w = POSITIONS[pcid].w;
        RENDER_SDL_RECTS[rsrid].h = POSITIONS[pcid].h;

        RENDER_SDL_RECTS[rsrid].x = floor(POSITIONS[pcid].x - (POSITIONS[pcid].w / 2));
        RENDER_SDL_RECTS[rsrid].y = floor(POSITIONS[pcid].y - (POSITIONS[pcid].h / 2));
    }
}

void render_update_range(int start, int end, SDL_Renderer *renderer) {
    int ret = 0;

    for (int eid = start; eid <= end; eid++) {
        int hcid = ENTITY_HEALTH_IDX[eid];
        int rcid = ENTITY_RENDER_IDX[eid];
        int srid = RENDER_SDL_RECT_IDX[rcid];

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
    int render_sdl_rect_i = 0;

    for (int i = 0; i < RENDER_MAX; i++) {
        RENDER_SDL_RECT_IDX[i] = render_sdl_rect_i;

        render_sdl_rect_i++;
    }
}

void collision_sdl_rect_idx_set_up() {
    int collision_sdl_rect_i = 0;

    for (int i = 0; i < COLLISION_MAX; i++) {
        COLLISION_SDL_RECT_IDX[i] = collision_sdl_rect_i;

        collision_sdl_rect_i++;
    }
}

void entity_set_up() {
    render_sdl_rect_idx_set_up();
    collision_sdl_rect_idx_set_up();

    int entity_i    = 0;
    int position_i   = 0;
    int movement_i   = 0;
    int render_i  = 0;
    int health_i    = 0;
    int shooting_i  = 0;
    int collision_i = 0;

    for (int i = 0; i < PLAYER_MAX; i++) {
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

    for (int i = 0; i < ENEMY_MAX; i++) {
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

    for (int i = 0; i < BULLET_MAX; i++) {
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

    int bullet_i = 0;

    for (int i = 0; i < PLAYER_MAX; i++) {
        for (int j = 0; j < BULLET_PER_PLAYER; j++) {
            PLAYER_BULLET_IDX[i][j] = bullet_i;

            bullet_i++;
        }
    }

    for (int i = 0; i < PARTICLE_MAX; i++) {
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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("ERROR: SDL_Init() (%s)", SDL_GetError());

        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("WAT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, 0);

    if (window == NULL) {
        SDL_Log("ERROR: SDL_CreateWindow() (%s)", SDL_GetError());

        SDL_Quit();

        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1,
        //SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
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

    bullet_init_all(PLAYER_BULLETS_W, PLAYER_BULLETS_H, PLAYER_BULLETS_V);

    player_init(
        0,
        WINDOW_W / 2,
        WINDOW_H / 2,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
        PLAYER_V
    );

    enemy_init_all(
        ENEMY_WIDTH,
        ENEMY_HEIGHT,
        ENEMY_V
    );

    particle_init_all(PARTICLE_WIDTH, PARTICLE_HEIGHT, PARTICLE_V);

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
