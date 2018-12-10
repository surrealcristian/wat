#include "wat.h"

tinymt32_t              TINYMT_STATE;
struct Keys             KEYS;
struct InputManager     INPUT_MANAGER;
struct Player           PLAYER;
struct Bullet           PLAYER_BULLETS[PLAYER_BULLETS_MAX];
struct BulletManager    PLAYER_BULLET_MANAGER;
struct Enemy            ENEMIES[ENEMIES_MAX];
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

    rand_init(
        &TINYMT_STATE,
        time(NULL)
    );

    bullet_manager_init(
        &PLAYER_BULLET_MANAGER,
        PLAYER_BULLETS,
        PLAYER_BULLETS_MAX,
        PLAYER_BULLETS_W,
        PLAYER_BULLETS_H,
        PLAYER_BULLETS_V
    );

    player_init(
        &PLAYER,
        &KEYS,
        &PLAYER_BULLET_MANAGER,
        WINDOW_W / 2,
        WINDOW_H / 2,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
        PLAYER_V
    );

    enemy_manager_init(
        &ENEMY_MANAGER,
        &TINYMT_STATE,
        ENEMIES,
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
        &PLAYER,
        PLAYER_BULLETS,
        PLAYER_BULLETS_MAX,
        ENEMIES,
        ENEMIES_MAX,
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
        &KEYS,
        &PLAYER,
        PLAYER_BULLETS,
        &PLAYER_BULLET_MANAGER,
        ENEMIES,
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
        &KEYS,
        &GAME,
        &PLAYER
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
