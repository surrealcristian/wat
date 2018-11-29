#include "in_game_state.h"

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
) {
    self->keys                  = keys;
    self->player                = player;
    self->player_bullets        = player_bullets;
    self->player_bullet_manager = player_bullet_manager;
    self->enemies               = enemies;
    self->enemy_manager         = enemy_manager;
    self->particles             = particles;
    self->particle_manager      = particle_manager;
    self->collision_manager     = collision_manager;
}

void in_game_state_update(
    struct InGameState *self
) {
    player_update(self->player);
    bullet_manager_update(self->player_bullet_manager);
    enemy_manager_update(self->enemy_manager);
    particle_manager_update(self->particle_manager);
    collision_manager_update(self->collision_manager);
}

void in_game_state_render(
    struct InGameState *self,
    SDL_Renderer       *renderer
) {
    particle_manager_render(self->particle_manager, renderer);
    bullet_manager_render(self->player_bullet_manager, renderer);
    enemy_manager_render(self->enemy_manager, renderer);
    player_render(self->player, renderer);
}
