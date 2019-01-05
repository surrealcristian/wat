#ifndef WAT_H
#define WAT_H

#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "tinymt32.h"


/* config.h */
#define PLAYER_MAX                 1
#define PLAYER_BULLET_MAX          (PLAYER_MAX * 128)
#define ENEMY_MAX                  64
#define PARTICLE_MAX               128

#define WINDOW_W 480
#define WINDOW_H 640

#define UPDATES_PER_SECOND 120.0
#define MS_PER_UPDATE 8.33 /* 1000 / 120.0 */
#define SLEEP_MS 2.08 /* 1000 / 480.0 */

enum GameState { GAME_STATE_WELCOME, GAME_STATE_IN_GAME, GAME_STATE_PAUSE };

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 32
#define PLAYER_V 512

#define PLAYER_BULLETS_MAX 128
#define PLAYER_BULLETS_INIT_N 5
#define PLAYER_BULLETS_W 8
#define PLAYER_BULLETS_H 128
#define PLAYER_BULLETS_V 2048

#define ENEMY_BULLET_MAX (ENEMY_MAX * 8)
#define ENEMY_BULLETS_INIT_N 1
#define ENEMY_BULLETS_W 8
#define ENEMY_BULLETS_H 8
#define ENEMY_BULLETS_V 512

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


/* vector.h start */
struct Vector {
    float x;
    float y;
};

void vector_set_xy(struct Vector *self, float x, float y);
float vector_length(struct Vector *v);
void vector_normalize(struct Vector *self);
float vector_way_to(struct Vector *a, struct Vector *b);
/* vector.h end */


/* keys.h start */
struct Keyboard {
    int left;
    int right;
    int up;
    int down;
    int z;
};
/* keys.h end */

/* ecs start */
struct Entity {
    /* Position */
    struct Vector pos_pos;
    int          pos_w;
    int          pos_h;

    /* Movement */
    float mov_dir;
    int   mov_vel;

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
void mov_player_input(struct Keyboard *kb, struct Entity *player);

void hea_init(struct Entity *e, int alive, int time_enabled, float time);
void hea_kill_out_of_range(struct Entity *e, float xmin, float xmax, float ymin, float ymax);
struct Entity *hea_get_dead(struct Entity *es, int n);

void col_sync(struct Entity *es, int n);

void ren_sync(struct Entity *es, int n);
void ren_update(struct Entity *es, int n, SDL_Color *color, SDL_Renderer *renderer);
/* ecs.h end */


/* util.h start */
#define PI 3.14159265358979323846264338327950288

double perf_counters_to_ms(Uint64 start, Uint64 end);

void rand_init(tinymt32_t *state, uint32_t seed);
int rand_n(tinymt32_t *state, int n);
float fclamp(float f, float min, float max);
float rad2deg(float rad);
float deg2rad(float deg);
/* util.h end */


/* text.h start */
enum TextSize { TEXT_SIZE_SMALL, TEXT_SIZE_MEDIUM, TEXT_SIZE_LARGE };

enum TextAlign { TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT };

#define RUNE_SMALL_PX  2
#define RUNE_MEDIUM_PX 4
#define RUNE_LARGE_PX  8

#define RUNE_W       5
#define RUNE_H       5
#define RUNE_SPACE_W 1

struct Text {
    SDL_Rect       rect;
    char           *value;
    enum TextSize  size;
    enum TextAlign align;
    float          x;
    float          y;
    int            value_len;
    int            size_px;
};

void text_init(
    struct Text    *text,
    char           *value,
    enum TextSize  size,
    enum TextAlign align,
    float          x,
    float          y
);

void text_set_value(struct Text *text, char *value);

void text_render_rune(
    struct Text  *self,
    SDL_Renderer *renderer,
    int          rune,
    float        x,
    float        y
);

void text_render(struct Text *self, SDL_Renderer *renderer);
/* text.h end */


/* player.h start */
void player_fire(struct Entity *player);

void player_fire_update(struct Entity *es, int n);
/* player.h end */


/* enemy.h start */
struct EnemyManager {
    float        time;
    float        spacing;
};

void enemy_spawn(struct EnemyManager *em, struct Entity *es);
/* enemy.h end */


/* score.h start */
struct Score {
    unsigned long value;
};
/* score.h end */


/* collision.h start */
void col_player_vs_enemies(struct Entity *players, int players_n, struct Entity *enemies, int enemies_n);
void col_enemies_vs_player_bullets(struct Entity *enemies, int enemies_n, struct Entity *bullets, int bullets_n);
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
    enum GameState state;
};

void game_init();
void game_run(SDL_Renderer *renderer);
/* game.h end */


/* input.h start */
void input_update(struct Keyboard *kb, SDL_Event *event, struct Entity *player);
/* input.h end */


void mov_fclamp_map(struct Entity *es, int n);
void hea_kill_out_of_map(struct Entity *es, int n);
void hea_kill_time(struct Entity *es, int n);

#endif
