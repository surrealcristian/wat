#ifndef WAT_H
#define WAT_H

#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "tinymt32.h"


/* config.h */
#define PLAYER_MAX                 1
#define BULLET_PER_PLAYER          128
#define BULLET_MAX                 (PLAYER_MAX * BULLET_PER_PLAYER)
#define ENEMY_MAX                  64
#define PARTICLE_MAX               128

#define WINDOW_W 480
#define WINDOW_H 640

#define UPDATES_PER_SECOND 120.0
#define MS_PER_UPDATE 8.33 /* 1000 / 120.0 */
#define SLEEP_MS 2.08 /* 1000 / 480.0 */

#define STATE_WELCOME 0
#define STATE_IN_GAME 1
#define STATE_PAUSE   2

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 32
#define PLAYER_V 512

#define PLAYER_BULLETS_MAX 128
#define PLAYER_BULLETS_INIT_N 5
#define PLAYER_BULLETS_W 8
#define PLAYER_BULLETS_H 128
#define PLAYER_BULLETS_V 2048

#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32
#define ENEMY_V 128

#define PARTICLE_WIDTH 8
#define PARTICLE_HEIGHT 8
#define PARTICLE_V 1024

#define EXPLOSION_PARTICLES_N 1

#define ENEMY_SCORE 10

extern float PLAYER_BULLETS_OFFSET_X[5];
extern float PLAYER_BULLETS_OFFSET_Y[5];

extern float EXPLOSION_PARTICLES_VX[4];
extern float EXPLOSION_PARTICLES_VY[4];
/* config.h */


/* vec2f.h start */
struct Vec2f {
    float x;
    float y;
};

void vec2f_set_xy(struct Vec2f *self, float x, float y);
float vec2f_length(struct Vec2f *v);
void vec2f_normalize(struct Vec2f *self);
/* vec2f.h end */

/* ecs start */
struct Entity {
    /* Position */
    struct Vec2f pos_pos;
    int          pos_w;
    int          pos_h;

    /* Movement */
    struct Vec2f mov_dir;
    int          mov_vel;

    /* Health */
    int   hea_time_enabled;
    float hea_time;
    int   hea_alive;

    /* Shooting */
    int   sho_bullets_n;
    int   sho_firing;
    int   sho_fire_spacing;
    float sho_fire_time;

    /* Collision */
    SDL_Rect col_sdl_rect;

    /* Render */
    SDL_Rect ren_sdl_rect;
};

void mov_init(struct Entity *e, float x, float y, int w, int h, int v);
void mov_update(struct Entity *es, int n);

void hea_init(struct Entity *e, int alive, int time_enabled, float time);
void hea_kill_out_of_range(struct Entity *e, float xmin, float xmax, float ymin, float ymax);
struct Entity *hea_get_dead(struct Entity *es, int n);

void col_sync(struct Entity *es, int n);

void ren_sync(struct Entity *es, int n);
void ren_update(struct Entity *es, int n, SDL_Color *color, SDL_Renderer *renderer);
/* ecs.h end */


/* util.h start */
#define PI 3.14159265358979323846264338327950288

double performance_counters_to_ms(Uint64 start, Uint64 end);
void rand_init(tinymt32_t *state, uint32_t seed);
int rand_n(tinymt32_t *state, int n);
float fclamp(float f, float min, float max);
/* util.h end */


/* text.h start */
#define TEXT_SIZE_SMALL  0
#define TEXT_SIZE_MEDIUM 1
#define TEXT_SIZE_LARGE  2

#define TEXT_ALIGN_LEFT   0
#define TEXT_ALIGN_CENTER 1
#define TEXT_ALIGN_RIGHT  2

#define RUNE_SMALL_PX  2
#define RUNE_MEDIUM_PX 4
#define RUNE_LARGE_PX  8

#define RUNE_W       5
#define RUNE_H       5
#define RUNE_SPACE_W 1

struct Text {
    SDL_Rect rect;
    char     *value;
    int      size;
    int      align;
    float    x;
    float    y;

    int      value_len;
    int      size_px;
};

void text_init(
    struct Text *text,
    char        *value,
    int         size,
    int         align,
    float       x,
    float       y
);

void text_set_value(struct Text *text, char *value);
void text_set_size(struct Text *text, int size);
void text_set_align(struct Text *text, int align);
void text_set_x(struct Text *text, float x);
void text_set_y(struct Text *text, float y);

void text_render_rune(
    struct Text  *self,
    SDL_Renderer *renderer,
    int          rune,
    float        x,
    float        y
);

void text_render(struct Text *self, SDL_Renderer *renderer);
/* text.h end */


/* keys.h start */
struct Keyboard {
    int left;
    int right;
    int up;
    int down;
    int z;
};
/* keys.h end */


/* player.h start */
void player_on_button_a_keydown(struct Entity *e);
void player_on_button_a_keyup(struct Entity *e);
void player_fire(struct Entity *player);

void player_fire_update();
/* player.h end */


/* enemy.h start */
struct EnemyManager {
    float        time;
    float        spacing;
};

void enemy_spawn();
/* enemy.h end */


/* score.h start */
struct Score {
    unsigned long value;
};

void score_init();
/* score.h end */


/* collision.h start */
void col_player_vs_enemies();
void col_enemies_vs_player_bullets();
void col_explode(struct Entity *e);
/* collision.h end */


/* welcome_state.h start */
void welcome_state_update();
void welcome_state_render(SDL_Renderer *renderer);
/* welcome_state.h end */


/* hud.h start */
#define HUD_SCORE_TEXT_BUFSIZE 32

struct HUD {
    struct Score *score;
    struct Text  *score_text;
};

void hud_update();
void hud_render(SDL_Renderer *renderer);
/* hud.h end */


/* in_game_state.h start */
void in_game_state_update();
void in_game_state_render(SDL_Renderer *renderer);
/* in_game_state.h end */


/* pause_state.h start */
void pause_state_update();
void pause_state_render(SDL_Renderer *renderer);
/* pause_state.h end */


/* game.h start */
struct Game {
    int state;
};

void game_init();
void game_run(SDL_Renderer *renderer);
/* game.h end */


/* input.h start */
void input_update();
/* input.h end */


void mov_fclamp_map(struct Entity *e);
void hea_kill_out_of_map(struct Entity *es, int n);
void hea_kill_time(struct Entity *es, int n);

#endif
