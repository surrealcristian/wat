#include "collision_manager.h"

void collision_manager_init(
    struct CollisionManager *self,
    struct ParticleManager  *particle_manager,
    struct Score            *score
) {
    self->particle_manager = particle_manager;
    self->player_bullets   = player_bullets;
    self->bullets_n        = bullets_n;
    self->enemies          = enemies;
    self->enemies_n        = enemies_n;
    self->score            = score;
}

void collision_manager_player_vs_enemies(
    struct CollisionManager *self
) {
    int peid = PLAYER_ENTITY_IDX[0];
    int ppid = ENTITY_PHYSICS_IDX[peid];
    int psrid = PHYSICS_SDL_RECT_IDX[ppid];

    if (PHYSICS[ppid].alive == 0) {
        return;
    }

    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eeid = ENEMY_ENTITY_IDX[i];
        int epid = ENTITY_PHYSICS_IDX[eeid];
        int esrid = PHYSICS_SDL_RECT_IDX[epid];

        if (PHYSICS[epid].alive == 0) {
            continue;
        }

        if (SDL_HasIntersection(&PHYSICS_SDL_RECTS[psrid], &PHYSICS_SDL_RECTS[esrid]) == SDL_TRUE) {
            PHYSICS[epid].alive = 0;
            continue;
        }
    }
}

void collision_manager_enemies_vs_player_bullets(
    struct CollisionManager *self
) {
    int peid = PLAYER_ENTITY_IDX[0];
    int ppid = ENTITY_PHYSICS_IDX[peid];
    int psrid = PHYSICS_SDL_RECT_IDX[ppid];

    for (int i = 0; i < ENEMIES_MAX; i++) {
        int eeid = ENEMY_ENTITY_IDX[i];
        int epid = ENTITY_PHYSICS_IDX[eeid];
        int esrid = PHYSICS_SDL_RECT_IDX[epid];

        if (PHYSICS[epid].alive == 0) {
            continue;
        }

        for (int j = 0; j < BULLET_PER_PLAYER; j++) {
            int beid = BULLET_ENTITY_IDX[j];
            int bpid = ENTITY_PHYSICS_IDX[beid];
            int bsrid = PHYSICS_SDL_RECT_IDX[bpid];

            if (PHYSICS[bpid].alive == 0) {
                continue;
            }

            if (SDL_HasIntersection(&PHYSICS_SDL_RECTS[esrid], &PHYSICS_SDL_RECTS[bsrid]) == SDL_TRUE) {
                PHYSICS[epid].alive = 0;
                PHYSICS[bpid].alive = 0;

                collision_manager_make_explosion(self, PHYSICS[epid].x, PHYSICS[epid].y);

                self->score->value += ENEMY_SCORE;

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
