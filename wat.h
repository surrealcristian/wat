#ifndef WAT_H
#define WAT_H

#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "tinymt32.h"

/* wat_dod.h start */
#define PLAYER_MAX                 1
#define BULLET_PER_PLAYER          128
#define BULLET_MAX                 (PLAYER_MAX * BULLET_PER_PLAYER)
#define ENEMY_MAX                  16
#define PARTICLE_MAX               128
#define ENTITY_MAX                 (PLAYER_MAX + BULLET_MAX + ENEMY_MAX + PARTICLE_MAX)

#define POSITION_MAX               (PLAYER_MAX + BULLET_MAX + ENEMY_MAX + PARTICLE_MAX)

#define MOVEMENT_MAX               (PLAYER_MAX + BULLET_MAX + ENEMY_MAX + PARTICLE_MAX)

#define HEALTH_MAX                 (PLAYER_MAX + BULLET_MAX + ENEMY_MAX + PARTICLE_MAX)

#define SHOOTING_MAX               PLAYER_MAX

#define COLLISION_MAX              (PLAYER_MAX + BULLET_MAX + ENEMY_MAX)

#define RENDER_SDL_RECT_PER_PLAYER   1
#define RENDER_SDL_RECT_PER_BULLET   1
#define RENDER_SDL_RECT_PER_ENEMY    1
#define RENDER_SDL_RECT_PER_PARTICLE 1
#define RENDER_MAX                   (PLAYER_MAX + BULLET_MAX + ENEMY_MAX + PARTICLE_MAX)
#define RENDER_SDL_RECT_MAX          ((PLAYER_MAX * RENDER_SDL_RECT_PER_PLAYER) + (BULLET_MAX * RENDER_SDL_RECT_PER_BULLET) + (ENEMY_MAX * RENDER_SDL_RECT_PER_ENEMY) + (PARTICLE_MAX * RENDER_SDL_RECT_PER_PARTICLE))


#define ENTITY_TYPE_PLAYER   0
#define ENTITY_TYPE_ENEMY    0
#define ENTITY_TYPE_BULLET   0
#define ENTITY_TYPE_PARTICLE 0


struct Entity {
    int type;
};

struct PositionComponent {
    float x;
    float y;
    int   w;
    int   h;
};

struct MovementComponent {
    int   v;
    int   vx;
    int   vy;
};

struct RenderComponent {
    float x;
    float y;
    int   w;
    int   h;
};

struct ShootingComponent {
    int   bullets_n;
    int   firing;
    int   fire_spacing;
    float fire_time; 
};

struct HealthComponent {
    int alive;
};
/* wat_dod.h end */


/* util.h start */
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


/* config.h start */
#define WINDOW_W 480
#define WINDOW_H 640

#define UPDATES_PER_SECOND 120.0
#define MS_PER_UPDATE 8.33 /* 1000 / 120.0 */
#define SLEEP_MS 2.08 /* 1000 / 480.0 */

#define STATE_WELCOME 0
#define STATE_IN_GAME 1
#define STATE_PAUSE   2

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define PLAYER_V 512

#define PLAYER_BULLETS_MAX 128
#define PLAYER_BULLETS_INIT_N 5
#define PLAYER_BULLETS_W 4
#define PLAYER_BULLETS_H 4
#define PLAYER_BULLETS_V 2048
#define PLAYER_BULLETS_VX +0
#define PLAYER_BULLETS_VY -1

#define ENEMIES_MAX 16
#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 32
#define ENEMY_V 128
#define ENEMY_VX +0
#define ENEMY_VY +1

#define PARTICLE_WIDTH 8
#define PARTICLE_HEIGHT 8
#define PARTICLE_V 1024

#define EXPLOSION_PARTICLES_N 4

#define ENEMY_SCORE 10

extern float PLAYER_BULLETS_OFFSET_X[5];
extern float PLAYER_BULLETS_OFFSET_Y[5];

extern float EXPLOSION_PARTICLES_VX[4];
extern float EXPLOSION_PARTICLES_VY[4];
/* config.h end */


/* player.h start */
void player_on_button_a_keydown(int index);
void player_on_button_a_keyup(int index);
void player_fire(int index);

void player_update(int index);
void player_fire_update(int index);
void player_update_all();
void player_fire_update_all();
/* player.h end */


/* enemy.h start */
struct EnemyManager {
    float        time;
    float        spacing;
};

void enemy_try_spawn();
void enemy_spawn();
/* enemy.h end */


/* score.h start */
struct Score {
    unsigned long value;
};

void score_init();
/* score.h end */


/* collision.h start */
void collision_player_vs_enemies();
void collision_enemies_vs_player_bullets();

void collision_make_explosion(float x, float y);

void collision_update();
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


void movement_init(int eid, float x, float y, int w, int h, int v);
void movement_fclamp_player(int eid);
void movement_update(int idx);
void movement_update_range(int start, int end);

void collision_sync_range(int start, int end);

void health_init(int eid, int alive);
void health_kill_if_out_of_range(int eid, float xmin, float xmax, float ymin, float ymax);
void health_kill_if_out_of_map_range(int start, int end);
int health_get_dead_range(int start, int end);

void render_sync_range(int start, int end);

void render_update_range(int start, int end, SDL_Renderer *renderer);

#endif
