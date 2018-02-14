#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480

typedef struct Sprite {
    float x;
    float y;
    int width;
    int height;
    int velocity;
    int angle;
    SDL_Rect *rect;
} Sprite;

Sprite *sprite_new(void);

void sprite_free(Sprite *self);

float sprite_get_x(Sprite *self);

void sprite_set_x(Sprite *self, float value);

float sprite_get_y(Sprite *self);

void sprite_set_y(Sprite *self, float value);

int sprite_get_width(Sprite *self);

void sprite_set_width(Sprite *self, int value);

int sprite_get_height(Sprite *self);

void sprite_set_height(Sprite *self, int value);

int sprite_get_velocity(Sprite *self);

void sprite_set_velocity(Sprite *self, int value);

int sprite_get_angle(Sprite *self);

void sprite_set_angle(Sprite *self, int value);

int sprite_render(Sprite *self, SDL_Renderer *renderer);

#endif /* SPRITE_H */
