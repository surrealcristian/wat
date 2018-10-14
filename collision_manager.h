#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "particle_manager.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"

struct CollisionManager {
    struct ParticleManager *particle_manager;
    struct Player *player;
    struct Bullet *player_bullets;
    int bullets_n;
    struct Enemy *enemies;
    int enemies_n;
};

void collision_manager_init(struct CollisionManager *self, struct ParticleManager *particle_manager, struct Player *player, struct Bullet *player_bullets, int bullets_n, struct Enemy *enemies, int enemies_n);
void collision_manager_player_vs_enemies(struct CollisionManager *self);
void collision_manager_enemies_vs_player_bullets(struct CollisionManager *self);
void collision_manager_make_explosion(struct CollisionManager *self, float x, float y);
void collision_manager_update(struct CollisionManager *self);

#endif
