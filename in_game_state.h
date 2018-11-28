#ifndef IN_GAME_STATE_H
#define IN_GAME_STATE_H

#include "SDL.h"
#include "keys.h"
#include "player.h"
#include "bullet.h"
#include "bullet_manager.h"
#include "enemy.h"
#include "enemy_manager.h"
#include "particle.h"
#include "particle_manager.h"
#include "collision_manager.h"

struct InGameState {
    struct Keys             *keys;
    struct Player           *player;
    struct Bullet           *player_bullets;
    struct BulletManager    *player_bullet_manager;
    struct Enemy            *enemies;
    struct EnemyManager     *enemy_manager;
    struct Particle         *particles;
    struct ParticleManager  *particle_manager;
    struct CollisionManager *collision_manager;
};

void in_game_state_init(
    struct InGameState      *self,
    struct Keys             *keys,
    struct Player           *player,
    struct Bullet           *player_bullets,
    struct BulletManager    *player_bullet_manager,
    struct Enemy            *enemies,
    struct EnemyManager     *enemy_manager,
    struct Particle         *particles,
    struct ParticleManager  *particle_manager,
    struct CollisionManager *collision_manager
);

void in_game_state_update(
    struct InGameState *self
);

void in_game_state_render(
    struct InGameState *self,
    SDL_Renderer       *renderer
);

#endif
