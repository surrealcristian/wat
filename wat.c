#include "wat.h"

// entity

struct PhysicsComponent  PHYSICS[PHYSICS_MAX];
struct GraphicsComponent GRAPHICS[GRAPHICS_MAX];
struct HealthComponent   HEALTHS[HEALTH_MAX];
struct ShootingComponent SHOOTING[SHOOTING_MAX];

SDL_Rect PHYSICS_SDL_RECTS[PHYSICS_SDL_RECT_MAX];
SDL_Rect GRAPHICS_SDL_RECTS[GRAPHICS_SDL_RECT_MAX];

int PHYSICS_SDL_RECT_IDX[PHYSICS_MAX];
int GRAPHICS_SDL_RECT_IDX[GRAPHICS_MAX];
int ENTITY_PHYSICS_IDX[ENTITY_MAX];
int ENTITY_GRAPHICS_IDX[ENTITY_MAX];
int ENTITY_HEALTH_IDX[ENTITY_MAX];
int ENTITY_SHOOTING_IDX[ENTITY_MAX];
int PLAYER_ENTITY_IDX[PLAYER_MAX];
int ENEMY_ENTITY_IDX[ENEMY_MAX];
int BULLET_ENTITY_IDX[BULLET_MAX];
int PLAYER_BULLET_IDX[PLAYER_MAX][BULLET_PER_PLAYER];


// particle

struct PhysicsComponent  PARTICLE_PHYSICS[PARTICLE_MAX];
struct GraphicsComponent PARTICLE_GRAPHICS[PARTICLE_MAX];
struct HealthComponent   PARTICLE_HEALTHS[PARTICLE_MAX];

SDL_Rect PARTICLE_PHYSICS_SDL_RECTS[PARTICLE_MAX];
SDL_Rect PARTICLE_GRAPHICS_SDL_RECTS[PARTICLE_MAX];

int PARTICLE_PHYSICS_SDL_RECT_IDX[PARTICLE_MAX];
int PARTICLE_GRAPHICS_SDL_RECT_IDX[PARTICLE_MAX];
int PARTICLE_PHYSICS_IDX[PARTICLE_MAX];
int PARTICLE_GRAPHICS_IDX[PARTICLE_MAX];
int PARTICLE_HEALTH_IDX[PARTICLE_MAX];


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
void bullet_init(int idx, float x, float y, int w, int h, int v) {
    int eid   = BULLET_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int hid   = ENTITY_HEALTH_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    PHYSICS[pid].w = w;
    PHYSICS[pid].h = h;

    PHYSICS_SDL_RECTS[psrid].w = PHYSICS[pid].w;
    PHYSICS_SDL_RECTS[psrid].h = PHYSICS[pid].h;

    GRAPHICS_SDL_RECTS[gsrid].w = PHYSICS[pid].w;
    GRAPHICS_SDL_RECTS[gsrid].h = PHYSICS[pid].h;

    bullet_set_x(idx, x);
    bullet_set_y(idx, y);

    PHYSICS[pid].v = v;

    HEALTHS[hid].alive = 0;
}

float bullet_get_x(int idx) {
    int eid = BULLET_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    return PHYSICS[pid].x;
}

void bullet_set_x(int idx, float value) {
    int eid   = BULLET_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int hid   = ENTITY_HEALTH_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    float x_min = 0 - PHYSICS[pid].w - 1;
    float x_max = WINDOW_W + PHYSICS[pid].w + 1;

    if (value < x_min || value > x_max) {
        HEALTHS[hid].alive = 0;

        value = 0;
        PHYSICS[pid].x = 0;
        PHYSICS[pid].y = 0;

        return;
    }

    PHYSICS[pid].x = value;

    PHYSICS_SDL_RECTS[psrid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
    GRAPHICS_SDL_RECTS[gsrid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
}

float bullet_get_y(int idx) {
    int eid = BULLET_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    return PHYSICS[pid].y;
}

void bullet_set_y(int idx, float value) {
    int eid   = BULLET_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int hid   = ENTITY_HEALTH_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    float y_min = 0 - PHYSICS[pid].h - 1;
    float y_max = WINDOW_H + PHYSICS[pid].h + 1;

    if (value < y_min || value > y_max) {
        HEALTHS[hid].alive = 0;

        value = 0;
        PHYSICS[pid].x = 0;
        PHYSICS[pid].y = 0;

        return;
    }

    PHYSICS[pid].y = value;

    PHYSICS_SDL_RECTS[psrid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
    GRAPHICS_SDL_RECTS[gsrid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
}

void bullet_update(int idx) {
    int eid  = BULLET_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];

    float x = PHYSICS[pid].x + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vx / UPDATES_PER_SECOND);
    float y = PHYSICS[pid].y + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vy / UPDATES_PER_SECOND);

    bullet_set_x(idx, x);
    bullet_set_y(idx, y);
}

void bullet_init_all(int w, int h, int v) {
    for (int i = 0; i < BULLET_MAX; i++) {
        bullet_init(i, 0, 0, w, h, v);
    }
}

int bullet_get_free() {
    for (int i = 0; i < BULLET_MAX; i++) {
        int eid = BULLET_ENTITY_IDX[i];
        int hid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hid].alive == 1) {
            continue;
        }

        return i;
    }

    return -1;
}

void bullet_update_all() {
    for (int i = 0; i < BULLET_MAX; i++) {
        int eid = BULLET_ENTITY_IDX[i];
        int hid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hid].alive == 0) {
            continue;
        }

        bullet_update(i);
    }
}
// bullet.c end


// player.c start
void player_init(int idx, float x, float y, int w, int h, int v) {
    int eid  = PLAYER_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];
    int gid  = ENTITY_GRAPHICS_IDX[eid];
    int hid  = ENTITY_HEALTH_IDX[eid];
    int sid  = ENTITY_SHOOTING_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    PHYSICS[pid].w = w;
    PHYSICS[pid].h = h;

    PHYSICS_SDL_RECTS[psrid].w = PHYSICS[pid].w;
    PHYSICS_SDL_RECTS[psrid].h = PHYSICS[pid].h;

    GRAPHICS_SDL_RECTS[gsrid].w = PHYSICS[pid].w;
    GRAPHICS_SDL_RECTS[gsrid].h = PHYSICS[pid].h;

    player_set_x(idx, x);
    player_set_y(idx, y);

    PHYSICS[pid].v = v;

    HEALTHS[hid].alive = 1;

    SHOOTING[sid].bullets_n = PLAYER_BULLETS_INIT_N;
    SHOOTING[sid].fire_spacing = 128;
}

void player_set_x(int idx, float value) {
    int eid   = PLAYER_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    float x_min = 0 + PHYSICS[pid].w / 2;
    float x_max = WINDOW_W - PHYSICS[pid].w / 2;

    if (value < x_min) {
        value = x_min;
    } else if (value > x_max) {
        value = x_max;
    }

    PHYSICS[pid].x = value;

    PHYSICS_SDL_RECTS[psrid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
    GRAPHICS_SDL_RECTS[gsrid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
}

void player_set_y(int idx, float value) {
    int eid   = PLAYER_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    float y_min = 0 + PHYSICS[pid].h / 2;
    float y_max = WINDOW_H - PHYSICS[pid].h / 2;

    if (value < y_min) {
        value = y_min;
    } else if (value > y_max) {
        value = y_max;
    }

    PHYSICS[pid].y = value;

    PHYSICS_SDL_RECTS[psrid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
    GRAPHICS_SDL_RECTS[gsrid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
}

void player_on_button_a_keydown(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int sid = ENTITY_SHOOTING_IDX[eid];

    SHOOTING[sid].firing = 1;
    SHOOTING[sid].fire_time = SHOOTING[sid].fire_spacing * 1.0;
}

void player_on_button_a_keyup(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int sid = ENTITY_SHOOTING_IDX[eid];

    SHOOTING[sid].firing = 0;
    SHOOTING[sid].fire_time = 0.0;
}

void player_fire(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];
    int sid = ENTITY_SHOOTING_IDX[eid];

    for (int i = 0; i < SHOOTING[sid].bullets_n; i++) {
        int bid = bullet_get_free();

        if (bid == -1) {
            return;
        }

        int beid = BULLET_ENTITY_IDX[bid];
        int bpid = ENTITY_PHYSICS_IDX[beid];
        int bhid = ENTITY_HEALTH_IDX[beid];

        HEALTHS[bhid].alive = 1;

        float x = PHYSICS[pid].x + PHYSICS[pid].w * PLAYER_BULLETS_OFFSET_X[i];
        float y = PHYSICS[pid].y + PHYSICS[pid].h * PLAYER_BULLETS_OFFSET_Y[i];

        bullet_set_x(bid, x);
        bullet_set_y(bid, y);

        PHYSICS[bpid].vx = PLAYER_BULLETS_VX;
        PHYSICS[bpid].vy = PLAYER_BULLETS_VY;
    }
}

void player_update(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];
    int sid = ENTITY_SHOOTING_IDX[eid];

    PHYSICS[pid].vx = 0;
    PHYSICS[pid].vy = 0;

    if (KEYBOARD.right) {
        PHYSICS[pid].vx = +1;
    }

    if (KEYBOARD.left) {
        PHYSICS[pid].vx = -1;
    }

    if (KEYBOARD.up) {
        PHYSICS[pid].vy = -1;
    }

    if (KEYBOARD.down) {
        PHYSICS[pid].vy = +1;
    }

    player_set_x(idx, PHYSICS[pid].x + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vx / UPDATES_PER_SECOND));
    player_set_y(idx, PHYSICS[pid].y + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vy / UPDATES_PER_SECOND));

    if (KEYBOARD.z) {
        SHOOTING[sid].fire_time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments
    }

    if (SHOOTING[sid].fire_time >= SHOOTING[sid].fire_spacing) {
        player_fire(idx);
        SHOOTING[sid].fire_time = 0.0;
    }
}

void player_update_all() {
    for (int i = 0; i < PLAYER_MAX; i++) {
        int eid = PLAYER_ENTITY_IDX[i];
        int hid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hid].alive == 0) {
            continue;
        }

        player_update(i);
    }
}
// player.c end


// enemy.c start
void enemy_init(int idx, float x, float y, int w, int h, int v) {
    int eid   = ENEMY_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int hid   = ENTITY_HEALTH_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    PHYSICS[pid].w = w;
    PHYSICS[pid].h = h;

    PHYSICS_SDL_RECTS[psrid].w = PHYSICS[pid].w;
    PHYSICS_SDL_RECTS[psrid].h = PHYSICS[pid].h;

    GRAPHICS_SDL_RECTS[gsrid].w = PHYSICS[pid].w;
    GRAPHICS_SDL_RECTS[gsrid].h = PHYSICS[pid].h;

    enemy_set_x(idx, x);
    enemy_set_y(idx, y);

    PHYSICS[pid].v = v;

    HEALTHS[hid].alive = 0;
}

void enemy_set_x(int idx, float value) {
    int eid   = ENEMY_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int hid   = ENTITY_HEALTH_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    float x_min = 0 - PHYSICS[pid].w + 1;
    float x_max = WINDOW_W + PHYSICS[pid].w + 1;

    if (value < x_min || value > x_max) {
        HEALTHS[hid].alive = 0;

        value = 0;
        PHYSICS[pid].x = 0;
        PHYSICS[pid].y = 0;

        return;
    }

    PHYSICS[pid].x = value;

    PHYSICS_SDL_RECTS[psrid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
    GRAPHICS_SDL_RECTS[gsrid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
}

void enemy_set_y(int idx, float value) {
    int eid  = ENEMY_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];
    int gid  = ENTITY_GRAPHICS_IDX[eid];
    int hid  = ENTITY_HEALTH_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    float y_min = 0 - PHYSICS[pid].h - 1;
    float y_max = WINDOW_H + PHYSICS[pid].h + 1;

    if (value < y_min || value > y_max) {
        HEALTHS[hid].alive = 0;

        value = 0;
        PHYSICS[pid].x = 0;
        PHYSICS[pid].y = 0;

        return;
    }

    PHYSICS[pid].y = value;

    PHYSICS_SDL_RECTS[psrid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
    GRAPHICS_SDL_RECTS[gsrid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
}

void enemy_update(int idx) {
    int eid = ENEMY_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    enemy_set_x(idx, PHYSICS[pid].x + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vx / UPDATES_PER_SECOND));
    enemy_set_y(idx, PHYSICS[pid].y + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vy / UPDATES_PER_SECOND));
}

void enemy_init_all(int w, int h, int v) {
    ENEMY_MANAGER.time    = 0.0;
    ENEMY_MANAGER.spacing = 100.0;

    for (int i = 0; i < ENEMIES_MAX; i++) {
        enemy_init(i, 0, 0, w, h, v);
    }
}

int enemy_get_free() {
    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eid = ENEMY_ENTITY_IDX[i];
        int hid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hid].alive == 1) {
            continue;
        }

        return i;
    }

    return -1;
}

void enemy_spawn() {
    int idx = enemy_get_free();

    if (idx == -1) {
        return;
    }

    int eid = ENEMY_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];
    int hid = ENTITY_HEALTH_IDX[eid];

    HEALTHS[hid].alive = 1;

    enemy_set_x(idx, rand_n(&TINYMT_STATE, WINDOW_W + 1));
    enemy_set_y(idx, 0 - PHYSICS[pid].h);

    PHYSICS[pid].vx = ENEMY_VX;
    PHYSICS[pid].vy = ENEMY_VY;
}

void enemy_update_all() {
    for (int i = 0; i < ENEMY_MAX; i++) {
        int eid = ENEMY_ENTITY_IDX[i];
        int hid = ENTITY_HEALTH_IDX[eid];

        if (HEALTHS[hid].alive == 0) {
            continue;
        }

        enemy_update(i);
    }

    ENEMY_MANAGER.time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments

    if (ENEMY_MANAGER.time >= ENEMY_MANAGER.spacing) {
        enemy_spawn();
        ENEMY_MANAGER.time = 0.0;
    }
}
// enemy.c end


// particle.c start
void particle_init(int i, float x, float y, int w, int h, int v) {
    int pid   = PARTICLE_PHYSICS_IDX[i];
    int gid   = PARTICLE_GRAPHICS_IDX[i];
    int hid   = PARTICLE_HEALTH_IDX[i];
    int psrid = PARTICLE_PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = PARTICLE_GRAPHICS_SDL_RECT_IDX[gid];

    PARTICLE_PHYSICS[pid].w = w;
    PARTICLE_PHYSICS[pid].h = h;

    PARTICLE_PHYSICS_SDL_RECTS[psrid].w = PARTICLE_PHYSICS[pid].w;
    PARTICLE_PHYSICS_SDL_RECTS[psrid].h = PARTICLE_PHYSICS[pid].h;

    PARTICLE_GRAPHICS_SDL_RECTS[gsrid].w = PARTICLE_PHYSICS[pid].w;
    PARTICLE_GRAPHICS_SDL_RECTS[gsrid].h = PARTICLE_PHYSICS[pid].h;

    particle_set_x(i, x);
    particle_set_y(i, y);

    PARTICLE_PHYSICS[pid].v = v;

    PARTICLE_HEALTHS[hid].alive = 0;
}

void particle_set_x(int i, float value) {
    int pid   = PARTICLE_PHYSICS_IDX[i];
    int gid   = PARTICLE_GRAPHICS_IDX[i];
    int hid   = PARTICLE_HEALTH_IDX[i];
    int psrid = PARTICLE_PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = PARTICLE_GRAPHICS_SDL_RECT_IDX[gid];

    float x_min = 0 - PARTICLE_PHYSICS[pid].w + 1;
    float x_max = WINDOW_W + PARTICLE_PHYSICS[pid].w + 1;

    if (value < x_min || value > x_max) {
        PARTICLE_HEALTHS[hid].alive = 0;
        return;
    }

    PARTICLE_PHYSICS[pid].x = value;

    PARTICLE_PHYSICS_SDL_RECTS[psrid].x  = floor(PARTICLE_PHYSICS[pid].x - (PARTICLE_PHYSICS[pid].w / 2));
    PARTICLE_GRAPHICS_SDL_RECTS[gsrid].x = floor(PARTICLE_PHYSICS[pid].x - (PARTICLE_PHYSICS[pid].w / 2));
}

void particle_set_y(int i, float value) {
    int pid = PARTICLE_PHYSICS_IDX[i];
    int gid = PARTICLE_GRAPHICS_IDX[i];
    int hid = PARTICLE_HEALTH_IDX[i];
    int psrid = PARTICLE_PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = PARTICLE_GRAPHICS_SDL_RECT_IDX[gid];

    float y_min = 0 - PARTICLE_PHYSICS[pid].h - 1;
    float y_max = WINDOW_H + PARTICLE_PHYSICS[pid].h + 1;

    if (value < y_min || value > y_max) {
        PARTICLE_HEALTHS[hid].alive = 0;
        return;
    }

    PARTICLE_PHYSICS[pid].y = value;

    PARTICLE_PHYSICS_SDL_RECTS[psrid].y  = floor(PARTICLE_PHYSICS[pid].y - (PARTICLE_PHYSICS[pid].h / 2));
    PARTICLE_GRAPHICS_SDL_RECTS[gsrid].y = floor(PARTICLE_PHYSICS[pid].y - (PARTICLE_PHYSICS[pid].h / 2));
}

void particle_update(int i) {
    int pid = PARTICLE_PHYSICS_IDX[i];

    particle_set_x(i, PARTICLE_PHYSICS[pid].x + (1.0 * PARTICLE_PHYSICS[pid].v * PARTICLE_PHYSICS[pid].vx / UPDATES_PER_SECOND));
    particle_set_y(i, PARTICLE_PHYSICS[pid].y + (1.0 * PARTICLE_PHYSICS[pid].v * PARTICLE_PHYSICS[pid].vy / UPDATES_PER_SECOND));
}

void particle_render(int i, SDL_Renderer *renderer) {
    int gid   = PARTICLE_GRAPHICS_IDX[i];
    int gsrid = PARTICLE_GRAPHICS_SDL_RECT_IDX[gid];

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &PARTICLE_GRAPHICS_SDL_RECTS[gsrid]);
}

void particle_init_all(int w, int h, int v) {
    for (int i = 0; i < PARTICLE_MAX; i++) {
        particle_init(i, 0, 0, w, h, v);
    }
}

int particle_get_free() {
    for (int i = 0; i < PARTICLE_MAX; i++) {
        int hid = PARTICLE_HEALTH_IDX[i];

        if (PARTICLE_HEALTHS[hid].alive == 1) {
            continue;
        }

        return i;
    }

    return -1;
}

void particle_update_all() {
    for (int i = 0; i < PARTICLE_MAX; i++) {
        int hid = PARTICLE_HEALTH_IDX[i];

        if (PARTICLE_HEALTHS[hid].alive == 0) {
            continue;
        }

        particle_update(i);
    }
}

void particle_render_all(SDL_Renderer *renderer) {
    for (int i = 0; i < PARTICLE_MAX; i++) {
        int hid = PARTICLE_HEALTH_IDX[i];

        if (PARTICLE_HEALTHS[hid].alive == 0) {
            continue;
        }

        particle_render(i, renderer);
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
    int peid = PLAYER_ENTITY_IDX[0];
    int ppid = ENTITY_PHYSICS_IDX[peid];
    int phid = ENTITY_HEALTH_IDX[peid];
    int psrid = PHYSICS_SDL_RECT_IDX[ppid];

    if (HEALTHS[phid].alive == 0) {
        return;
    }

    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eeid = ENEMY_ENTITY_IDX[i];
        int epid = ENTITY_PHYSICS_IDX[eeid];
        int ehid = ENTITY_HEALTH_IDX[eeid];
        int esrid = PHYSICS_SDL_RECT_IDX[epid];

        if (HEALTHS[ehid].alive == 0) {
            continue;
        }

        if (SDL_HasIntersection(&PHYSICS_SDL_RECTS[psrid], &PHYSICS_SDL_RECTS[esrid]) == SDL_TRUE) {
            HEALTHS[ehid].alive = 0;
            continue;
        }
    }
}

void collision_enemies_vs_player_bullets() {
    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eeid = ENEMY_ENTITY_IDX[i];
        int epid = ENTITY_PHYSICS_IDX[eeid];
        int ehid = ENTITY_HEALTH_IDX[eeid];
        int esrid = PHYSICS_SDL_RECT_IDX[epid];

        if (HEALTHS[ehid].alive == 0) {
            continue;
        }

        for (int j = 0; j < BULLET_PER_PLAYER; j++) {
            int beid = BULLET_ENTITY_IDX[j];
            int bpid = ENTITY_PHYSICS_IDX[beid];
            int bhid = ENTITY_HEALTH_IDX[beid];
            int bsrid = PHYSICS_SDL_RECT_IDX[bpid];

            if (HEALTHS[bhid].alive == 0) {
                continue;
            }

            if (SDL_HasIntersection(&PHYSICS_SDL_RECTS[esrid], &PHYSICS_SDL_RECTS[bsrid]) == SDL_TRUE) {
                HEALTHS[ehid].alive = 0;
                HEALTHS[bhid].alive = 0;

                collision_make_explosion(PHYSICS[epid].x, PHYSICS[epid].y);

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
        int idx = particle_get_free();

        if (idx == -1) {
            return;
        }

        int pid = PARTICLE_PHYSICS_IDX[idx];
        int hid = PARTICLE_HEALTH_IDX[idx];

        particle_set_x(idx, x);
        particle_set_y(idx, y);

        PARTICLE_PHYSICS[pid].vx = EXPLOSION_PARTICLES_VX[i];
        PARTICLE_PHYSICS[pid].vy = EXPLOSION_PARTICLES_VY[i];

        PARTICLE_HEALTHS[hid].alive = 1;
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
    bullet_update_all();
    enemy_update_all();
    particle_update_all();
    collision_update();
    hud_update();
}

void in_game_state_render(SDL_Renderer *renderer) {
    particle_render_all(renderer);

    entity_render_all(renderer);

    hud_render(renderer);
}
// in_game_state.c end


// pause_state.c start
void pause_state_update() {

}

void pause_state_render(SDL_Renderer *renderer) {
    particle_render_all(renderer);

    entity_render_all(renderer);

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




void entity_render_all(SDL_Renderer *renderer) {
    int ret = 0;

    for (int eid = 0; eid < ENTITY_MAX; eid++) {
        int gid  = ENTITY_GRAPHICS_IDX[eid];
        int hid  = ENTITY_HEALTH_IDX[eid];
        int srid = GRAPHICS_SDL_RECT_IDX[gid];

        if (HEALTHS[hid].alive == 0) {
            continue;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

        ret = SDL_RenderFillRect(renderer, &GRAPHICS_SDL_RECTS[srid]);

        if (ret < 0) {
            SDL_Log("ERROR: SDL_RenderFillRect() (%s)", SDL_GetError());
        }
    }
}

void physics_sdl_rect_idx_set_up() {
    int physics_sdl_rect_i = 0;

    for (int i = 0; i < PHYSICS_MAX; i++) {
        PHYSICS_SDL_RECT_IDX[i] = physics_sdl_rect_i;

        physics_sdl_rect_i++;
    }
}

void graphics_sdl_rect_idx_set_up() {
    int graphics_sdl_rect_i = 0;

    for (int i = 0; i < GRAPHICS_MAX; i++) {
        GRAPHICS_SDL_RECT_IDX[i] = graphics_sdl_rect_i;

        graphics_sdl_rect_i++;
    }
}

void particle_physics_sdl_rect_idx_set_up() {
    int particle_physics_sdl_rect_i = 0;

    for (int i = 0; i < PARTICLE_MAX; i++) {
        PARTICLE_PHYSICS_SDL_RECT_IDX[i] = particle_physics_sdl_rect_i;

        particle_physics_sdl_rect_i++;
    }

}

void particle_graphics_sdl_rect_idx_set_up() {
    int particle_graphics_sdl_rect_i = 0;

    for (int i = 0; i < PARTICLE_MAX; i++) {
        PARTICLE_GRAPHICS_SDL_RECT_IDX[i] = particle_graphics_sdl_rect_i;

        particle_graphics_sdl_rect_i++;
    }
}

void entity_set_up() {
    physics_sdl_rect_idx_set_up();
    graphics_sdl_rect_idx_set_up();

    int entity_i   = 0;
    int physics_i  = 0;
    int graphics_i = 0;
    int health_i   = 0;
    int shooting_i = 0;

    for (int i = 0; i < PLAYER_MAX; i++) {
        PLAYER_ENTITY_IDX[i] = entity_i;

        ENTITY_PHYSICS_IDX[entity_i]  = physics_i;
        ENTITY_GRAPHICS_IDX[entity_i] = graphics_i;
        ENTITY_HEALTH_IDX[entity_i]   = health_i;
        ENTITY_SHOOTING_IDX[entity_i] = shooting_i;
        
        entity_i++;
        physics_i++;
        graphics_i++;
        health_i++;
        shooting_i++;
    }

    for (int i = 0; i < ENEMY_MAX; i++) {
        ENEMY_ENTITY_IDX[i] = entity_i;

        ENTITY_PHYSICS_IDX[entity_i]  = physics_i;
        ENTITY_GRAPHICS_IDX[entity_i] = graphics_i;
        ENTITY_HEALTH_IDX[entity_i]   = health_i;
        ENTITY_SHOOTING_IDX[entity_i] = shooting_i;

        entity_i++;
        physics_i++;
        graphics_i++;
        health_i++;
        shooting_i++;
    }

    for (int i = 0; i < BULLET_MAX; i++) {
        BULLET_ENTITY_IDX[i] = entity_i;

        ENTITY_PHYSICS_IDX[entity_i]  = physics_i;
        ENTITY_GRAPHICS_IDX[entity_i] = graphics_i;
        ENTITY_HEALTH_IDX[entity_i]   = health_i;
        ENTITY_SHOOTING_IDX[entity_i] = -1;

        entity_i++;
        physics_i++;
        graphics_i++;
        health_i++;
    }

    int bullet_i = 0;

    for (int i = 0; i < PLAYER_MAX; i++) {
        for (int j = 0; j < BULLET_PER_PLAYER; j++) {
            PLAYER_BULLET_IDX[i][j] = bullet_i;

            bullet_i++;
        }
    }
}

void particle_set_up() {
    particle_physics_sdl_rect_idx_set_up();
    particle_graphics_sdl_rect_idx_set_up();

    int particle_i = 0;
    int physics_i  = 0;
    int graphics_i = 0;
    int health_i = 0;

    for (int i = 0; i < PARTICLE_MAX; i++) {
        PARTICLE_PHYSICS_IDX[particle_i]  = physics_i;
        PARTICLE_GRAPHICS_IDX[particle_i] = graphics_i;
        PARTICLE_HEALTH_IDX[particle_i] = health_i;
        
        particle_i++;
        physics_i++;
        graphics_i++;
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

    particle_set_up();

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
