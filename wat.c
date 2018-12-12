#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "tinymt32.h"


// HEADER_START


// wat_dod.h start
#define PLAYER_MAX                   1
#define BULLET_PER_PLAYER            128
#define BULLET_MAX                   PLAYER_MAX * BULLET_PER_PLAYER
#define ENEMY_MAX                    16
#define ENTITY_MAX                   PLAYER_MAX + BULLET_MAX + ENEMY_MAX

#define PHYSICS_SDL_RECT_PER_PLAYER  1
#define PHYSICS_SDL_RECT_PER_BULLET  1
#define PHYSICS_SDL_RECT_PER_ENEMY   1
#define PHYSICS_MAX                  PLAYER_MAX + BULLET_MAX + ENEMY_MAX
#define PHYSICS_SDL_RECT_MAX         (PLAYER_MAX * PHYSICS_SDL_RECT_PER_PLAYER) + (BULLET_MAX * PHYSICS_SDL_RECT_PER_BULLET) + (ENEMY_MAX + PHYSICS_SDL_RECT_PER_ENEMY)
#define PHYSICS_SDL_RECT_PER_ENTITY  1

#define GRAPHICS_SDL_RECT_PER_PLAYER 1
#define GRAPHICS_SDL_RECT_PER_BULLET 1
#define GRAPHICS_SDL_RECT_PER_ENEMY  1
#define GRAPHICS_MAX                 PLAYER_MAX + BULLET_MAX + ENEMY_MAX
#define GRAPHICS_SDL_RECT_MAX        (PLAYER_MAX * GRAPHICS_SDL_RECT_PER_PLAYER) + (BULLET_MAX * GRAPHICS_SDL_RECT_PER_BULLET) + (ENEMY_MAX * GRAPHICS_SDL_RECT_PER_ENEMY)
#define GRAPHICS_SDL_RECT_PER_ENTITY 1

#define SHOOTING_MAX                 PLAYER_MAX


// PhysicsComponent

struct PhysicsComponent {
    float x;
    float y;
    int   w;
    int   h;
    int   v;
    int   vx;
    int   vy;
    int   alive;
};

// GraphicsComponent

struct GraphicsComponent {
    float x;
    float y;
    int   w;
    int   h;
};

// ShootingComponent

struct ShootingComponent {
    int   bullets_n;
    int   firing;
    int   fire_spacing;
    float fire_time; 
};

// Entity

struct Entity {
};

struct EntityPhysicsComponent {
    int entity_id;
    int physics_id;
};

struct EntityShootingComponent {
    int entity_id;
    int shooting_id;
};
// wat_dod.h end


// util.h start
double performance_counters_to_ms(
    Uint64 start,
    Uint64 end
);

void rand_init(
    tinymt32_t *state,
    uint32_t   seed
);

int rand_n(
    tinymt32_t *state,
    int        n
);
// util.h end


// text.h start
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

void text_set_value(
    struct Text *text,
    char        *value
);

void text_set_size(
    struct Text *text,
    int         size
);

void text_set_align(
    struct Text *text,
    int         align
);

void text_set_x(
    struct Text *text,
    float       x
);

void text_set_y(
    struct Text *text,
    float       y
);

void text_render_rune(
    struct Text  *self,
    SDL_Renderer *renderer,
    int          rune,
    float        x,
    float        y
);

void text_render(
    struct Text  *self,
    SDL_Renderer *renderer
);
// text.h end


// keys.h start
struct Keys {
    int left;
    int right;
    int up;
    int down;
    int z;
};
// keys.h end


// config.h start
#define WINDOW_W 480
#define WINDOW_H 640

#define UPDATES_PER_SECOND 120.0
#define MS_PER_UPDATE 8.33 // 1000 / 120.0
#define SLEEP_MS 2.08 // 1000 / 480.0

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

#define PARTICLES_MAX 128
#define PARTICLE_WIDTH 8
#define PARTICLE_HEIGHT 8
#define PARTICLE_V 1024

#define EXPLOSION_PARTICLES_N 4

#define ENEMY_SCORE 10

extern float PLAYER_BULLETS_OFFSET_X[5];
extern float PLAYER_BULLETS_OFFSET_Y[5];

extern float EXPLOSION_PARTICLES_VX[4];
extern float EXPLOSION_PARTICLES_VY[4];
// config.h end


// bullet_manager.h start
void bullet_manager_init(
    int   index,
    float x,
    float y,
    int   w,
    int   h,
    int   v
);

float bullet_manager_get_x(
    int index
);

void bullet_manager_set_x(
    int   index,
    float value
);

float bullet_manager_get_y(
    int index
);

void bullet_manager_set_y(
    int   index,
    float value
);

void bullet_manager_update(
    int index
);

void bullet_manager_render(
    int          index,
    SDL_Renderer *renderer
);

void bullet_manager_init_all(
    int                  w,
    int                  h,
    int                  v
);

int bullet_manager_get_free();

void bullet_manager_update_all();

void bullet_manager_render_all(
    SDL_Renderer *renderer
);
// bullet_manager.h end


// player_manager.h start
void player_manager_init(
    int    index,
    float  x,
    float  y,
    int    w,
    int    h,
    int    v
);

void player_manager_set_x(
    int   index,
    float value
);

void player_manager_set_y(
    int   index,
    float value
);

void player_manager_on_button_a_keydown(
    int index
);

void player_manager_on_button_a_keyup(
    int index
);

void player_manager_fire(
    int index
);

void player_manager_update(
    int index
);
// player_manager.h end


// enemy_manager.h start
void enemy_manager_init(
    int   idx,
    float x,
    float y,
    int   w,
    int   h,
    int   v
);

void enemy_manager_set_x(
    int   idx,
    float value
);

void enemy_manager_set_y(
    int   idx,
    float value
);


void enemy_manager_update(
    int idx
);

void enemy_manager_render(
    int          idx,
    SDL_Renderer *renderer
);

struct EnemyManager {
    tinymt32_t   *rand_state;
    struct Enemy *enemies;
    int          n;
    float        time;
    float        spacing;
};

void enemy_manager_init_all(
    struct EnemyManager *self,
    tinymt32_t          *rand_state,
    int                 n,
    int                 w,
    int                 h,
    int                 v
);


int enemy_manager_get_free(
    struct EnemyManager *self
);

void enemy_manager_spawn(
    struct EnemyManager *self
);

void enemy_manager_update_all(
    struct EnemyManager *self
);

void enemy_manager_render_all(
    struct EnemyManager *self,
    SDL_Renderer        *renderer
);
// enemy_manager.h end


// particle.h start
struct Particle {
    float    x;
    float    y;
    int      w;
    int      h;
    int      v;
    int      vx;
    int      vy;
    int      alive;
    SDL_Rect rect;
};

void particle_init(
    struct Particle *self,
    float  x,
    float  y,
    int    w,
    int    h,
    int    v
);

void particle_set_x(
    struct Particle *self,
    float           value
);

void particle_set_y(
    struct Particle *self,
    float           value
);

void particle_update(
    struct Particle *self
);

void particle_render(
    struct Particle *self,
    SDL_Renderer    *renderer
);
// particle.h end


// score.h start
struct Score {
    unsigned long value;
};

void score_init(
    struct Score *self
);
// score.h end


// particle_manager.h start
struct ParticleManager {
    struct Particle *particles;
    int             n;
};

void particle_manager_init(
    struct ParticleManager *self,
    struct Particle        *particles,
    int                    n,
    int                    w,
    int                    h,
    int                    v
);

struct Particle *particle_manager_get_free(
    struct ParticleManager *self
);

void particle_manager_update(
    struct ParticleManager *self
);

void particle_manager_render(
    struct ParticleManager *self,
    SDL_Renderer           *renderer
);
// particle_manager.h end


// collision_manager.h start
struct CollisionManager {
    struct ParticleManager *particle_manager;
    struct Player          *player;
    struct Bullet          *player_bullets;
    int                    bullets_n;
    struct Enemy           *enemies;
    int                    enemies_n;
    struct Score           *score;
};

void collision_manager_init(
    struct CollisionManager *self,
    struct ParticleManager  *particle_manager,
    struct Score            *score
);

void collision_manager_player_vs_enemies(
    struct CollisionManager *self
);

void collision_manager_enemies_vs_player_bullets(
    struct CollisionManager *self
);

void collision_manager_make_explosion(
    struct CollisionManager *self,
    float                   x,
    float                   y
);

void collision_manager_update(
    struct CollisionManager *self
);
// collision_manager.h end


// welcome_state.h start
struct WelcomeState {
    struct Text *text;
};

void welcome_state_init(
    struct WelcomeState *self,
    struct Text         *text
);

void welcome_state_update(
    struct WelcomeState *self
);

void welcome_state_render(
    struct WelcomeState *self,
    SDL_Renderer        *renderer
);
// welcome_state.h end


// hud.h start
#define HUD_SCORE_TEXT_BUFSIZE 32

struct HUD {
    struct Score *score;
    struct Text  *score_text;
};

void hud_init(
    struct HUD   *self,
    struct Score *score,
    struct Text  *score_text
);

void hud_update(
    struct HUD *self
);

void hud_render(
    struct HUD   *self,
    SDL_Renderer *renderer
);
// hud.h end


// in_game_state.h start
struct InGameState {
    struct EnemyManager     *enemy_manager;
    struct Particle         *particles;
    struct ParticleManager  *particle_manager;
    struct CollisionManager *collision_manager;
    struct Score            *score;
    struct HUD              *hud;
};

void in_game_state_init(
    struct InGameState      *self,
    struct EnemyManager     *enemy_manager,
    struct Particle         *particles,
    struct ParticleManager  *particle_manager,
    struct CollisionManager *collision_manager,
    struct Score            *score,
    struct HUD              *hud
);

void in_game_state_update(
    struct InGameState *self
);

void in_game_state_render(
    struct InGameState *self,
    SDL_Renderer       *renderer
);
// in_game_state.h end


// pause_state.h start
struct PauseState {
    struct Text *text;
};

void pause_state_init(
    struct PauseState *self,
    struct Text       *text
);

void pause_state_update(
    struct PauseState *self
);

void pause_state_render(
    struct PauseState *self,
    SDL_Renderer      *renderer
);
// pause_state.h end


// game.h start
struct Game {
    struct InputManager *input_manager;
    struct WelcomeState   *welcome_state;
    struct InGameState    *in_game_state;
    struct PauseState     *pause_state;
    int                   state;
};

void game_init(
    struct Game         *self,
    struct WelcomeState *welcome_state,
    struct InGameState  *in_game_state,
    struct PauseState   *pause_state
);

void game_set_input_manager(
    struct Game           *self,
    struct InputManager *input_manager
);

void game_run(
    struct Game  *self,
    SDL_Renderer *renderer
);
// game.h end


// input_manager.h start
struct InputManager {
    struct Game   *game;
};

void input_manager_init(
    struct InputManager *self,
    struct Game           *game
);

void input_manager_update(
    struct InputManager *self,
    SDL_Event             *event
);
// input_manager.h end


// HEADER_END

















































struct PhysicsComponent PHYSICS[PHYSICS_MAX];
SDL_Rect                PHYSICS_SDL_RECTS[PHYSICS_SDL_RECT_MAX];
int                     PHYSICS_SDL_RECT_IDX[PHYSICS_MAX];

struct GraphicsComponent GRAPHICS[GRAPHICS_MAX];
SDL_Rect                 GRAPHICS_SDL_RECTS[GRAPHICS_SDL_RECT_MAX];
int                      GRAPHICS_SDL_RECT_IDX[GRAPHICS_MAX];

struct ShootingComponent SHOOTING[SHOOTING_MAX];

struct Entity ENTITIES[ENTITY_MAX];

int ENTITY_PHYSICS_IDX[ENTITY_MAX];
int ENTITY_GRAPHICS_IDX[ENTITY_MAX];
int ENTITY_SHOOTING_IDX[ENTITY_MAX];

int PLAYER_ENTITY_IDX[PLAYER_MAX];
int ENEMY_ENTITY_IDX[ENEMY_MAX];
int BULLET_ENTITY_IDX[BULLET_MAX];

int PLAYER_BULLET_IDX[PLAYER_MAX][BULLET_PER_PLAYER];


#include "wat.h"

tinymt32_t              TINYMT_STATE;
struct Keys             KEYS;
struct InputManager     INPUT_MANAGER;
struct EnemyManager     ENEMY_MANAGER;
struct Particle         PARTICLES[PARTICLES_MAX];
struct ParticleManager  PARTICLE_MANAGER;
struct Score            SCORE;
struct CollisionManager COLLISION_MANAGER;
struct Text             WELCOME_TEXT;
struct WelcomeState     WELCOME_STATE;
struct Text             HUD_TEXT;
struct HUD              HUD;
struct InGameState      IN_GAME_STATE;
struct Text             PAUSE_TEXT;
struct PauseState       PAUSE_STATE;
struct Game             GAME;


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
    }

    int bullet_i = 0;

    for (int i = 0; i < PLAYER_MAX; i++) {
        for (int j = 0; j < BULLET_PER_PLAYER; j++) {
            PLAYER_BULLET_IDX[i][j] = bullet_i;

            bullet_i++;
        }
    }

    rand_init(
        &TINYMT_STATE,
        time(NULL)
    );

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
        ENEMIES_MAX,
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

    score_init(
        &SCORE
    );

    collision_manager_init(
        &COLLISION_MANAGER,
        &PARTICLE_MANAGER,
        &SCORE
    );

    text_init(
        &HUD_TEXT,
        "0",
        TEXT_SIZE_SMALL,
        TEXT_ALIGN_RIGHT,
        floor(WINDOW_W - WINDOW_W * 0.05),
        floor(0 + WINDOW_H * 0.05)
    );

    hud_init(
        &HUD,
        &SCORE,
        &HUD_TEXT
    );

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

    pause_state_init(
        &PAUSE_STATE,
        &PAUSE_TEXT
    );

    text_init(
        &WELCOME_TEXT,
        "WAT",
        TEXT_SIZE_LARGE,
        TEXT_ALIGN_CENTER,
        floor(WINDOW_W / 2),
        floor(WINDOW_H / 2)
    );

    welcome_state_init(    
        &WELCOME_STATE,
        &WELCOME_TEXT
    );

    game_init(
        &GAME,
        &WELCOME_STATE,
        &IN_GAME_STATE,
        &PAUSE_STATE
    );

    input_manager_init(
        &INPUT_MANAGER,
        &GAME
    );

    game_set_input_manager(
        &GAME,
        &INPUT_MANAGER
    );

    game_run(
        &GAME,
        renderer
    );

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
