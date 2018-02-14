#ifndef ACTOR_H
#define ACTOR_H

#include "SDL.h"

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480

typedef struct Actor {
    float x;
    float y;
    int width;
    int height;
    int velocity;
    int angle;
    SDL_Rect *rect;
} Actor;

Actor *actor_new(void);

void actor_free(Actor *self);

float actor_get_x(Actor *self);

void actor_set_x(Actor *self, float value);

float actor_get_y(Actor *self);

void actor_set_y(Actor *self, float value);

int actor_get_width(Actor *self);

void actor_set_width(Actor *self, int value);

int actor_get_height(Actor *self);

void actor_set_height(Actor *self, int value);

int actor_get_velocity(Actor *self);

void actor_set_velocity(Actor *self, int value);

int actor_get_angle(Actor *self);

void actor_set_angle(Actor *self, int value);

int actor_render(Actor *self, SDL_Renderer *renderer);

#endif /* ACTOR_H */