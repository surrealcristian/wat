#ifndef IN_GAME_STATE_H
#define IN_GAME_STATE_H

#include "SDL.h"
#include "keys.h"
#include "bullet_manager.h"
#include "enemy_manager.h"
#include "particle.h"
#include "particle_manager.h"
#include "collision_manager.h"
#include "score.h"
#include "hud.h"

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

#endif
