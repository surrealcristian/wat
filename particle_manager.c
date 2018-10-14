#include "particle_manager.h"

void particle_manager_init(struct ParticleManager *self, struct Particle *particles, int n, int w, int h, int v) {
    self->particles = particles;
    self->n = n;

    for (int i = 0; i < self->n; i++) {
        particle_init(&self->particles[i], 0, 0, w, h, v);
    }
}

struct Particle *particle_manager_get_free(struct ParticleManager *self) {
    struct Particle *particle = NULL;

    for (int i = 0; i < self->n; i++) {
        if (self->particles[i].alive == 1) {
            continue;
        }

        particle = &self->particles[i];

        break;
    }

    return particle;
}

void particle_manager_update(struct ParticleManager *self) {
    for (int i = 0; i < self->n; i++) {
        if (self->particles[i].alive == 0) {
            continue;
        }

        particle_update(&self->particles[i]);
    }
}

void particle_manager_render(struct ParticleManager *self, SDL_Renderer *renderer) {
    for (int i = 0; i < self->n; i++) {
        if (self->particles[i].alive == 0) {
            continue;
        }

        particle_render(&self->particles[i], renderer);
    }
}
