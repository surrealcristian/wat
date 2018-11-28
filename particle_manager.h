#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "SDL.h"
#include "particle.h"

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

#endif
