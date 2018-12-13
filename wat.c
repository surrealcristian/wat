#include "wat.h"

struct PhysicsComponent  PHYSICS[PHYSICS_MAX];
SDL_Rect                 PHYSICS_SDL_RECTS[PHYSICS_SDL_RECT_MAX];
int                      PHYSICS_SDL_RECT_IDX[PHYSICS_MAX];

struct GraphicsComponent GRAPHICS[GRAPHICS_MAX];
SDL_Rect                 GRAPHICS_SDL_RECTS[GRAPHICS_SDL_RECT_MAX];
int                      GRAPHICS_SDL_RECT_IDX[GRAPHICS_MAX];

struct ShootingComponent SHOOTING[SHOOTING_MAX];

struct Entity            ENTITIES[ENTITY_MAX];

int                      ENTITY_PHYSICS_IDX[ENTITY_MAX];
int                      ENTITY_GRAPHICS_IDX[ENTITY_MAX];
int                      ENTITY_SHOOTING_IDX[ENTITY_MAX];

int                      PLAYER_ENTITY_IDX[PLAYER_MAX];
int                      ENEMY_ENTITY_IDX[ENEMY_MAX];
int                      BULLET_ENTITY_IDX[BULLET_MAX];

int                      PLAYER_BULLET_IDX[PLAYER_MAX][BULLET_PER_PLAYER];


tinymt32_t               TINYMT_STATE;
struct Keys              KEYS;
struct InputManager      INPUT_MANAGER;
struct EnemyManager      ENEMY_MANAGER;
struct Particle          PARTICLES[PARTICLES_MAX];
struct ParticleManager   PARTICLE_MANAGER;
struct Score             SCORE;
struct CollisionManager  COLLISION_MANAGER;
struct Text              WELCOME_TEXT;
struct WelcomeState      WELCOME_STATE;
struct Text              HUD_TEXT;
struct HUD               HUD;
struct InGameState       IN_GAME_STATE;
struct Text              PAUSE_TEXT;
struct PauseState        PAUSE_STATE;
struct Game              GAME;


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


// bullet_manager.c start
void bullet_manager_init(
    int   idx,
    float x,
    float y,
    int   w,
    int   h,
    int   v
) {
    int eid   = BULLET_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    PHYSICS[pid].w = w;
    PHYSICS[pid].h = h;

    PHYSICS_SDL_RECTS[psrid].w = PHYSICS[pid].w;
    PHYSICS_SDL_RECTS[psrid].h = PHYSICS[pid].h;

    GRAPHICS_SDL_RECTS[gsrid].w = PHYSICS[pid].w;
    GRAPHICS_SDL_RECTS[gsrid].h = PHYSICS[pid].h;

    bullet_manager_set_x(idx, x);
    bullet_manager_set_y(idx, y);

    PHYSICS[pid].v = v;

    PHYSICS[pid].alive = 0;
}

float bullet_manager_get_x(int idx) {
    int eid = BULLET_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    return PHYSICS[pid].x;
}

void bullet_manager_set_x(int   idx, float value) {
    int eid   = BULLET_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    float x_min = 0 - PHYSICS[pid].w - 1;
    float x_max = WINDOW_W + PHYSICS[pid].w + 1;

    if (value < x_min || value > x_max) {
        PHYSICS[pid].alive = 0;
        return;
    }

    PHYSICS[pid].x = value;

    PHYSICS_SDL_RECTS[psrid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
    GRAPHICS_SDL_RECTS[gsrid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
}

float bullet_manager_get_y(int idx) {
    int eid = BULLET_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    return PHYSICS[pid].y;
}

void bullet_manager_set_y(int   idx, float value) {
    int eid   = BULLET_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    float y_min = 0 - PHYSICS[pid].h - 1;
    float y_max = WINDOW_H + PHYSICS[pid].h + 1;

    if (value < y_min || value > y_max) {
        PHYSICS[pid].alive = 0;
        return;
    }

    PHYSICS[pid].y = value;

    PHYSICS_SDL_RECTS[psrid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
    GRAPHICS_SDL_RECTS[gsrid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
}

void bullet_manager_update(int idx) {
    int eid  = BULLET_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];

    float x = PHYSICS[pid].x + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vx / UPDATES_PER_SECOND);
    float y = PHYSICS[pid].y + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vy / UPDATES_PER_SECOND);

    bullet_manager_set_x(idx, x);
    bullet_manager_set_y(idx, y);
}

void bullet_manager_render(int idx, SDL_Renderer *renderer) {
    int eid  = BULLET_ENTITY_IDX[idx];
    int gid  = ENTITY_GRAPHICS_IDX[eid];
    int srid = GRAPHICS_SDL_RECT_IDX[gid];

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &GRAPHICS_SDL_RECTS[srid]);
}

void bullet_manager_init_all(int w, int h, int v) {
    for (int i = 0; i < BULLET_MAX; i++) {
        bullet_manager_init(i, 0, 0, w, h, v);
    }
}

int bullet_manager_get_free() {
    for (int i = 0; i < BULLET_MAX; i++) {
        int eid = BULLET_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 1) {
            continue;
        }

        return i;
    }

    return -1;
}

void bullet_manager_update_all() {
    for (int i = 0; i < BULLET_MAX; i++) {
        int eid = BULLET_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 0) {
            continue;
        }

        bullet_manager_update(i);
    }
}

void bullet_manager_render_all(SDL_Renderer *renderer) {
    for (int i = 0; i < BULLET_MAX; i++) {
        int eid = BULLET_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 0) {
            continue;
        }

        bullet_manager_render(i, renderer);
    }
}
// bullet_manager.c end


// player_manager.c start
void player_manager_init(
    int   idx,
    float x,
    float y,
    int   w,
    int   h,
    int   v
) {
    int eid  = PLAYER_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];
    int gid  = ENTITY_GRAPHICS_IDX[eid];
    int sid  = ENTITY_SHOOTING_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    PHYSICS[pid].w = w;
    PHYSICS[pid].h = h;

    PHYSICS_SDL_RECTS[psrid].w = PHYSICS[pid].w;
    PHYSICS_SDL_RECTS[psrid].h = PHYSICS[pid].h;

    GRAPHICS_SDL_RECTS[gsrid].w = PHYSICS[pid].w;
    GRAPHICS_SDL_RECTS[gsrid].h = PHYSICS[pid].h;

    player_manager_set_x(idx, x);
    player_manager_set_y(idx, y);

    PHYSICS[pid].v = v;

    PHYSICS[pid].alive = 1;

    SHOOTING[sid].bullets_n = PLAYER_BULLETS_INIT_N;
    SHOOTING[sid].fire_spacing = 128;
}

void player_manager_set_x(int idx, float value) {
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

void player_manager_set_y(int idx, float value) {
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

void player_manager_on_button_a_keydown(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int sid = ENTITY_SHOOTING_IDX[eid];

    SHOOTING[sid].firing = 1;
    SHOOTING[sid].fire_time = SHOOTING[sid].fire_spacing * 1.0;
}

void player_manager_on_button_a_keyup(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int sid = ENTITY_SHOOTING_IDX[eid];

    SHOOTING[sid].firing = 0;
    SHOOTING[sid].fire_time = 0.0;
}

void player_manager_fire(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];
    int sid = ENTITY_SHOOTING_IDX[eid];

    for (int i = 0; i < SHOOTING[sid].bullets_n; i++) {
        int bid = bullet_manager_get_free();

        if (bid == -1) {
            return;
        }

        int beid = BULLET_ENTITY_IDX[bid];
        int bpid = ENTITY_PHYSICS_IDX[beid];

        PHYSICS[bpid].alive = 1;

        float x = PHYSICS[pid].x + PHYSICS[pid].w * PLAYER_BULLETS_OFFSET_X[i];
        float y = PHYSICS[pid].y + PHYSICS[pid].h * PLAYER_BULLETS_OFFSET_Y[i];

        bullet_manager_set_x(bid, x);
        bullet_manager_set_y(bid, y);

        PHYSICS[bpid].vx = PLAYER_BULLETS_VX;
        PHYSICS[bpid].vy = PLAYER_BULLETS_VY;
    }
}

void player_manager_update(int idx) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];
    int sid = ENTITY_SHOOTING_IDX[eid];

    PHYSICS[pid].vx = 0;
    PHYSICS[pid].vy = 0;

    if (KEYS.right) {
        PHYSICS[pid].vx = +1;
    }

    if (KEYS.left) {
        PHYSICS[pid].vx = -1;
    }

    if (KEYS.up) {
        PHYSICS[pid].vy = -1;
    }

    if (KEYS.down) {
        PHYSICS[pid].vy = +1;
    }

    player_manager_set_x(idx, PHYSICS[pid].x + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vx / UPDATES_PER_SECOND));
    player_manager_set_y(idx, PHYSICS[pid].y + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vy / UPDATES_PER_SECOND));

    if (KEYS.z) {
        SHOOTING[sid].fire_time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments
    }

    if (SHOOTING[sid].fire_time >= SHOOTING[sid].fire_spacing) {
        player_manager_fire(idx);
        SHOOTING[sid].fire_time = 0.0;
    }
}

void player_manager_render(int idx, SDL_Renderer  *renderer) {
    int eid = PLAYER_ENTITY_IDX[idx];
    int gid = ENTITY_GRAPHICS_IDX[eid];
    int srid = GRAPHICS_SDL_RECT_IDX[gid];

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &GRAPHICS_SDL_RECTS[srid]);
}
// player_manager.c end


// enemy_manager.c start
void enemy_manager_init(
    int   idx,
    float x,
    float y,
    int   w,
    int   h,
    int   v
) {
    int eid   = ENEMY_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    PHYSICS[pid].w = w;
    PHYSICS[pid].h = h;

    PHYSICS_SDL_RECTS[psrid].w = PHYSICS[pid].w;
    PHYSICS_SDL_RECTS[psrid].h = PHYSICS[pid].h;

    GRAPHICS_SDL_RECTS[gsrid].w = PHYSICS[pid].w;
    GRAPHICS_SDL_RECTS[gsrid].h = PHYSICS[pid].h;

    enemy_manager_set_x(idx, x);
    enemy_manager_set_y(idx, y);

    PHYSICS[pid].v = v;

    PHYSICS[pid].alive = 0;
}

void enemy_manager_set_x(int idx, float value) {
    int eid   = ENEMY_ENTITY_IDX[idx];
    int pid   = ENTITY_PHYSICS_IDX[eid];
    int gid   = ENTITY_GRAPHICS_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    float x_min = 0 - PHYSICS[pid].w + 1;
    float x_max = WINDOW_W + PHYSICS[pid].w + 1;

    if (value < x_min || value > x_max) {
        PHYSICS[pid].alive = 0;
        return;
    }

    PHYSICS[pid].x = value;

    PHYSICS_SDL_RECTS[psrid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
    GRAPHICS_SDL_RECTS[gsrid].x = floor(PHYSICS[pid].x - (PHYSICS[pid].w / 2));
}

void enemy_manager_set_y(int idx, float value) {
    int eid  = ENEMY_ENTITY_IDX[idx];
    int pid  = ENTITY_PHYSICS_IDX[eid];
    int gid  = ENTITY_GRAPHICS_IDX[eid];
    int psrid = PHYSICS_SDL_RECT_IDX[pid];
    int gsrid = GRAPHICS_SDL_RECT_IDX[gid];

    float y_min = 0 - PHYSICS[pid].h - 1;
    float y_max = WINDOW_H + PHYSICS[pid].h + 1;

    if (value < y_min || value > y_max) {
        PHYSICS[pid].alive = 0;
        return;
    }

    PHYSICS[pid].y = value;

    PHYSICS_SDL_RECTS[psrid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
    GRAPHICS_SDL_RECTS[gsrid].y = floor(PHYSICS[pid].y - (PHYSICS[pid].h / 2));
}

void enemy_manager_update(int idx) {
    int eid = ENEMY_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    enemy_manager_set_x(idx, PHYSICS[pid].x + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vx / UPDATES_PER_SECOND));
    enemy_manager_set_y(idx, PHYSICS[pid].y + (1.0 * PHYSICS[pid].v * PHYSICS[pid].vy / UPDATES_PER_SECOND));
}

void enemy_manager_render(int idx, SDL_Renderer *renderer) {
    int eid  = ENEMY_ENTITY_IDX[idx];
    int gid  = ENTITY_GRAPHICS_IDX[eid];
    int srid = GRAPHICS_SDL_RECT_IDX[gid];

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &GRAPHICS_SDL_RECTS[srid]);
}

void enemy_manager_init_all(
    struct EnemyManager *self,
    tinymt32_t          *rand_state,
    int                 w,
    int                 h,
    int                 v
) {
    self->rand_state = rand_state;

    self->time    = 0.0;
    self->spacing = 100.0;

    for (int i = 0; i < ENEMIES_MAX; i++) {
        enemy_manager_init(i, 0, 0, w, h, v);
    }
}

int enemy_manager_get_free(struct EnemyManager *self) {
    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eid = ENEMY_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 1) {
            continue;
        }

        return i;
    }

    return -1;
}

void enemy_manager_spawn(struct EnemyManager *self) {
    int idx = enemy_manager_get_free(self);

    if (idx == -1) {
        return;
    }

    int eid = ENEMY_ENTITY_IDX[idx];
    int pid = ENTITY_PHYSICS_IDX[eid];

    PHYSICS[pid].alive = 1;

    enemy_manager_set_x(idx, rand_n(self->rand_state, WINDOW_W + 1));
    enemy_manager_set_y(idx, 0 - PHYSICS[pid].h);

    PHYSICS[pid].vx = ENEMY_VX;
    PHYSICS[pid].vy = ENEMY_VY;
}

void enemy_manager_update_all(struct EnemyManager *self) {
    for (int i = 0; i < ENEMY_MAX; i++) {
        int eid = ENEMY_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 0) {
            continue;
        }

        enemy_manager_update(i);
    }

    self->time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments

    if (self->time >= self->spacing) {
        enemy_manager_spawn(self);
        self->time = 0.0;
    }
}

void enemy_manager_render_all(SDL_Renderer *renderer) {
    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eid = ENEMY_ENTITY_IDX[i];
        int pid = ENTITY_PHYSICS_IDX[eid];

        if (PHYSICS[pid].alive == 0) {
            continue;
        }

        enemy_manager_render(i, renderer);
    }
}
// enemy_manager.c end


// particle.c start
void particle_init(
    struct Particle *self,
    float           x,
    float           y,
    int             w,
    int             h,
    int             v
) {
    self->w = w;
    self->h = h;

    self->rect.w = self->w;
    self->rect.h = self->h;

    particle_set_x(self, x);
    particle_set_y(self, y);

    self->v = v;

    self->alive = 0;
}

void particle_set_x(struct Particle *self, float value) {
    float x_min = 0 - self->w + 1;
    float x_max = WINDOW_W + self->w + 1;

    if (value < x_min || value > x_max) {
        self->alive = 0;
        return;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

void particle_set_y(struct Particle *self, float value) {
    float y_min = 0 - self->h - 1;
    float y_max = WINDOW_H + self->h + 1;

    if (value < y_min || value > y_max) {
        self->alive = 0;
        return;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

void particle_update(struct Particle *self) {
    particle_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    particle_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));
}

void particle_render(struct Particle *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
// particle.c end


// score.c start
void score_init(struct Score *self) {
    self->value = 0;
}
// score.c end


// particle_manager.c start
void particle_manager_init(
    struct ParticleManager *self,
    struct Particle        *particles,
    int                    n,
    int                    w,
    int                    h,
    int                    v
) {
    self->particles = particles;
    self->n         = n;

    for (int i = 0; i < self->n; i++) {
        particle_init(&self->particles[i], 0, 0, w, h, v);
    }
}

struct Particle *particle_manager_get_free(struct ParticleManager *self) {
    struct Particle *particle = NULL;

    for (int i = 0; i < self->n; i++) {
        if (self->particles[i].alive == 1) {
            continue;
        }

        particle = &self->particles[i];

        break;
    }

    return particle;
}

void particle_manager_update(struct ParticleManager *self) {
    for (int i = 0; i < self->n; i++) {
        if (self->particles[i].alive == 0) {
            continue;
        }

        particle_update(&self->particles[i]);
    }
}

void particle_manager_render(
    struct ParticleManager *self,
    SDL_Renderer           *renderer
) {
    for (int i = 0; i < self->n; i++) {
        if (self->particles[i].alive == 0) {
            continue;
        }

        particle_render(&self->particles[i], renderer);
    }
}
// particle_manager.c end


// collision_manager.c start
void collision_manager_init(
    struct CollisionManager *self,
    struct ParticleManager  *particle_manager,
    struct Score            *score
) {
    self->particle_manager = particle_manager;
    self->score            = score;
}

void collision_manager_player_vs_enemies(struct CollisionManager *self) {
    int peid = PLAYER_ENTITY_IDX[0];
    int ppid = ENTITY_PHYSICS_IDX[peid];
    int psrid = PHYSICS_SDL_RECT_IDX[ppid];

    if (PHYSICS[ppid].alive == 0) {
        return;
    }

    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eeid = ENEMY_ENTITY_IDX[i];
        int epid = ENTITY_PHYSICS_IDX[eeid];
        int esrid = PHYSICS_SDL_RECT_IDX[epid];

        if (PHYSICS[epid].alive == 0) {
            continue;
        }

        if (SDL_HasIntersection(&PHYSICS_SDL_RECTS[psrid], &PHYSICS_SDL_RECTS[esrid]) == SDL_TRUE) {
            PHYSICS[epid].alive = 0;
            continue;
        }
    }
}

void collision_manager_enemies_vs_player_bullets(
    struct CollisionManager *self
) {
    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eeid = ENEMY_ENTITY_IDX[i];
        int epid = ENTITY_PHYSICS_IDX[eeid];
        int esrid = PHYSICS_SDL_RECT_IDX[epid];

        if (PHYSICS[epid].alive == 0) {
            continue;
        }

        for (int j = 0; j < BULLET_PER_PLAYER; j++) {
            int beid = BULLET_ENTITY_IDX[j];
            int bpid = ENTITY_PHYSICS_IDX[beid];
            int bsrid = PHYSICS_SDL_RECT_IDX[bpid];

            if (PHYSICS[bpid].alive == 0) {
                continue;
            }

            if (SDL_HasIntersection(&PHYSICS_SDL_RECTS[esrid], &PHYSICS_SDL_RECTS[bsrid]) == SDL_TRUE) {
                PHYSICS[epid].alive = 0;
                PHYSICS[bpid].alive = 0;

                collision_manager_make_explosion(self, PHYSICS[epid].x, PHYSICS[epid].y);

                self->score->value += ENEMY_SCORE;

                break;
            }
        }
    }
}

void collision_manager_update(struct CollisionManager *self) {
    collision_manager_player_vs_enemies(self);
    collision_manager_enemies_vs_player_bullets(self);
}

void collision_manager_make_explosion(
    struct CollisionManager *self,
    float                   x,
    float                   y
) {
    for (int i = 0; i < EXPLOSION_PARTICLES_N; i++) {
        struct Particle *particle = particle_manager_get_free(self->particle_manager);

        particle_set_x(particle, x);
        particle_set_y(particle, y);

        particle->vx = EXPLOSION_PARTICLES_VX[i];
        particle->vy = EXPLOSION_PARTICLES_VY[i];

        particle->alive = 1;
    }
}
// collision_manager.c end


// welcome_state.c start
void welcome_state_init(struct WelcomeState *self, struct Text *text) {
    self->text = text;
}

void welcome_state_update(struct WelcomeState *self) {

}

void welcome_state_render(
    struct WelcomeState *self,
    SDL_Renderer        *renderer
) {
    text_render(self->text, renderer);
}
// welcome_state.c end


// hud.c start
char HUD_SCORE_TEXT[HUD_SCORE_TEXT_BUFSIZE];

void hud_init(struct HUD *self, struct Score *score, struct Text *score_text) {
    self->score      = score;
    self->score_text = score_text;
}

void hud_update(struct HUD *self) {
    snprintf(HUD_SCORE_TEXT, HUD_SCORE_TEXT_BUFSIZE, "%lu", self->score->value);

    text_set_value(self->score_text, HUD_SCORE_TEXT);
}

void hud_render(struct HUD *self, SDL_Renderer *renderer) {
    text_render(self->score_text, renderer);
}
// hud.c end


// in_game_state.c start
void in_game_state_init(
    struct InGameState      *self,
    struct EnemyManager     *enemy_manager,
    struct Particle         *particles,
    struct ParticleManager  *particle_manager,
    struct CollisionManager *collision_manager,
    struct Score            *score,
    struct HUD              *hud
) {
    self->enemy_manager         = enemy_manager;
    self->particles             = particles;
    self->particle_manager      = particle_manager;
    self->collision_manager     = collision_manager;
    self->score                 = score;
    self->hud                   = hud;
}

void in_game_state_update(struct InGameState *self) {
    player_manager_update(0);
    bullet_manager_update_all();
    enemy_manager_update_all(self->enemy_manager);
    particle_manager_update(self->particle_manager);
    collision_manager_update(self->collision_manager);
    hud_update(self->hud);
}

void in_game_state_render(struct InGameState *self, SDL_Renderer *renderer) {
    particle_manager_render(self->particle_manager, renderer);
    bullet_manager_render_all(renderer);
    enemy_manager_render_all(renderer);
    player_manager_render(0, renderer);
    hud_render(self->hud, renderer);
}
// in_game_state.c end


// pause_state.c start
void pause_state_init( struct PauseState *self, struct Text *text) {
    self->text = text;
}

void pause_state_update(struct PauseState *self) {

}

void pause_state_render( struct PauseState *self, SDL_Renderer *renderer) {
    text_render(self->text, renderer);
}
// pause_state.c end


// game.c start
void game_init(
    struct Game         *self,
    struct WelcomeState *welcome_state,
    struct InGameState  *in_game_state,
    struct PauseState   *pause_state
) {
    self->welcome_state = welcome_state;
    self->in_game_state = in_game_state;
    self->pause_state   = pause_state;

    self->state = STATE_WELCOME;
}

void game_set_input_manager(
    struct Game           *self,
    struct InputManager *input_manager
) {
    self->input_manager = input_manager;
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

            input_manager_update(self->input_manager, &event);
        }

        while (lag >= MS_PER_UPDATE) {
            if (self->state == STATE_WELCOME) {
                welcome_state_update(self->welcome_state);
            } else if (self->state == STATE_IN_GAME) {
                in_game_state_update(self->in_game_state);
            } else if (self->state == STATE_PAUSE) {
                pause_state_update(self->pause_state);
            }

            lag -= MS_PER_UPDATE;
        }

        // RENDER

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // grey
        SDL_RenderClear(renderer);

        if (self->state == STATE_WELCOME) {
            welcome_state_render(self->welcome_state, renderer);
        } else if (self->state == STATE_IN_GAME) {
            in_game_state_render(self->in_game_state, renderer);
        } else if (self->state == STATE_PAUSE) {
            pause_state_render(self->pause_state, renderer);
        }

        SDL_RenderPresent(renderer);

        //SDL_Delay(SLEEP_MS);
    }
}
// game.c end


// input_manager.c start
void input_manager_init( struct InputManager *self, struct Game *game) {
    self->game   = game;
}

void input_manager_update(struct InputManager *self, SDL_Event *event) {
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

            if (self->game->state == STATE_IN_GAME) {
                player_manager_on_button_a_keydown(0);
            }
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

            if (self->game->state == STATE_WELCOME) {
                self->game->state = STATE_IN_GAME;
            } else if (self->game->state == STATE_IN_GAME) {
                player_manager_on_button_a_keyup(0);
            }
        } else if (sym == SDLK_ESCAPE) {
            if (self->game->state == STATE_IN_GAME) {
                self->game->state = STATE_PAUSE;
            } else if (self->game->state == STATE_PAUSE) {
                self->game->state = STATE_IN_GAME;
            }
        }
    }
}
// input_manager.c end


void init_physics_sdl_rect_idx() {
    int physics_sdl_rect_i = 0;

    for (int i = 0; i < PHYSICS_MAX; i++) {
        PHYSICS_SDL_RECT_IDX[i] = physics_sdl_rect_i;

        physics_sdl_rect_i++;
    }
}

void init_graphics_sdl_rect_idx() {
    int graphics_sdl_rect_i = 0;

    for (int i = 0; i < GRAPHICS_MAX; i++) {
        GRAPHICS_SDL_RECT_IDX[i] = graphics_sdl_rect_i;

        graphics_sdl_rect_i++;
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

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_Log("ERROR: SDL_CreateRenderer() (%s)", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    init_physics_sdl_rect_idx();
    init_graphics_sdl_rect_idx();

    int physics_i             = 0;
    int graphics_i            = 0;
    int shooting_i            = 0;
    int entity_i                        = 0;

    for (int i = 0; i < PLAYER_MAX; i++) {
        PLAYER_ENTITY_IDX[i] = entity_i;

        ENTITY_PHYSICS_IDX[entity_i]  = physics_i;
        ENTITY_GRAPHICS_IDX[entity_i] = graphics_i;
        ENTITY_SHOOTING_IDX[entity_i] = shooting_i;
        
        entity_i++;
        physics_i++;
        graphics_i++;
        shooting_i++;
    }

    for (int i = 0; i < ENEMY_MAX; i++) {
        ENEMY_ENTITY_IDX[i] = entity_i;

        ENTITY_PHYSICS_IDX[entity_i]  = physics_i;
        ENTITY_GRAPHICS_IDX[entity_i] = graphics_i;
        ENTITY_SHOOTING_IDX[entity_i] = shooting_i;

        entity_i++;
        physics_i++;
        graphics_i++;
        shooting_i++;
    }

    for (int i = 0; i < BULLET_MAX; i++) {
        BULLET_ENTITY_IDX[i] = entity_i;

        ENTITY_PHYSICS_IDX[entity_i]  = physics_i;
        ENTITY_GRAPHICS_IDX[entity_i] = graphics_i;
        ENTITY_SHOOTING_IDX[entity_i] = shooting_i;

        entity_i++;
        physics_i++;
        graphics_i++;
    }

    int bullet_i = 0;

    for (int i = 0; i < PLAYER_MAX; i++) {
        for (int j = 0; j < BULLET_PER_PLAYER; j++) {
            PLAYER_BULLET_IDX[i][j] = bullet_i;

            bullet_i++;
        }
    }

    rand_init(&TINYMT_STATE, time(NULL));

    bullet_manager_init_all(
        PLAYER_BULLETS_W,
        PLAYER_BULLETS_H,
        PLAYER_BULLETS_V
    );

    player_manager_init(
        0,
        WINDOW_W / 2,
        WINDOW_H / 2,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
        PLAYER_V
    );

    enemy_manager_init_all(
        &ENEMY_MANAGER,
        &TINYMT_STATE,
        ENEMY_WIDTH,
        ENEMY_HEIGHT,
        ENEMY_V
    );

    particle_manager_init(
        &PARTICLE_MANAGER,
        PARTICLES,
        PARTICLES_MAX,
        PARTICLE_WIDTH,
        PARTICLE_HEIGHT,
        PARTICLE_V
    );

    score_init(&SCORE);

    collision_manager_init(&COLLISION_MANAGER, &PARTICLE_MANAGER, &SCORE);

    text_init(
        &HUD_TEXT,
        "0",
        TEXT_SIZE_SMALL,
        TEXT_ALIGN_RIGHT,
        floor(WINDOW_W - WINDOW_W * 0.05),
        floor(0 + WINDOW_H * 0.05)
    );

    hud_init(&HUD, &SCORE, &HUD_TEXT);

    in_game_state_init(
        &IN_GAME_STATE,
        &ENEMY_MANAGER,
        PARTICLES,
        &PARTICLE_MANAGER,
        &COLLISION_MANAGER,
        &SCORE,
        &HUD
    );

    text_init(
        &PAUSE_TEXT,
        "PAUSE",
        TEXT_SIZE_LARGE,
        TEXT_ALIGN_CENTER,
        floor(WINDOW_W / 2),
        floor(WINDOW_H / 2)
    );

    pause_state_init(&PAUSE_STATE, &PAUSE_TEXT);

    text_init(
        &WELCOME_TEXT,
        "WAT",
        TEXT_SIZE_LARGE,
        TEXT_ALIGN_CENTER,
        floor(WINDOW_W / 2),
        floor(WINDOW_H / 2)
    );

    welcome_state_init(&WELCOME_STATE, &WELCOME_TEXT);

    game_init(&GAME, &WELCOME_STATE, &IN_GAME_STATE, &PAUSE_STATE);

    input_manager_init(&INPUT_MANAGER, &GAME);

    game_set_input_manager(&GAME, &INPUT_MANAGER);

    game_run(&GAME, renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
