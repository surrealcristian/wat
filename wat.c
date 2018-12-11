#include "wat_dod.h"
#include "SDL.h"


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
