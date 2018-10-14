#ifndef PARTICLE_H
#define PARTICLE_H

#include "SDL.h"
#include "config.h"

struct Particle {
    float x;
    float y;
    int w;
    int h;
    int v;
    int vx;
    int vy;
    int alive;
    SDL_Rect rect;
};

void particle_init(struct Particle *self, float x, float y, int w, int h, int v);
void particle_set_x(struct Particle *self, float value);
void particle_set_y(struct Particle *self, float value);
void particle_update(struct Particle *self);
void particle_render(struct Particle *self, SDL_Renderer *renderer);

#endif
