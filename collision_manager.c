#include "collision_manager.h"

void collision_manager_init(
    struct CollisionManager *self,
    struct ParticleManager  *particle_manager,
    struct Player           *player,
    struct Bullet           *player_bullets,
    int                     bullets_n,
    struct Enemy            *enemies,
    int                     enemies_n
) {
    self->particle_manager = particle_manager;
    self->player = player;
    self->player_bullets = player_bullets;
    self->bullets_n = bullets_n;
    self->enemies = enemies;
    self->enemies_n = enemies_n;
}

void collision_manager_player_vs_enemies(
    struct CollisionManager *self
) {
    if (self->player->alive == 0) {
        return;
    }

    for (int i = 0; i < ENEMIES_MAX; i++) {
        if (self->enemies->alive == 0) {
            continue;
        }

        if (SDL_HasIntersection(&self->player->rect, &self->enemies[i].rect) == SDL_TRUE) {
            self->enemies[i].alive = 0;
            continue;
        }
    }
}

void collision_manager_enemies_vs_player_bullets(
    struct CollisionManager *self
) {
    for (int i = 0; i < ENEMIES_MAX; i++) {
        if (self->enemies[i].alive == 0) {
            continue;
        }

        for (int j = 0; j < PLAYER_BULLETS_MAX; j++) {
            if (self->player_bullets[j].alive == 0) {
                continue;
            }

            if (SDL_HasIntersection(&self->enemies[i].rect, &self->player_bullets[j].rect) == SDL_TRUE) {
                self->enemies[i].alive = 0;
                self->player_bullets[j].alive = 0;

                collision_manager_make_explosion(self, self->enemies[i].x, self->enemies[i].y);
                break;
            }
        }
    }
}

void collision_manager_update(
    struct CollisionManager *self
) {
    collision_manager_player_vs_enemies(self);
    collision_manager_enemies_vs_player_bullets(self);
}

void collision_manager_make_explosion(
    struct CollisionManager *self,
    float                   x,
    float                   y
) {
    for (int i = 0; i < EXPLOSION_PARTICLES_N; i++) {
        struct Particle *particle = particle_manager_get_free(self->particle_manager);

        particle_set_x(particle, x);
        particle_set_y(particle, y);

        particle->vx = EXPLOSION_PARTICLES_VX[i];
        particle->vy = EXPLOSION_PARTICLES_VY[i];

        particle->alive = 1;
    }
}
