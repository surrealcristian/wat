#ifndef RECT_H
#define RECT_H

#include "SDL.h"
#include "sdl_rect.h"

typedef struct Rect {
    float x;
    float y;
    SDL_Rect *sdl_rect;
} Rect;

Rect *rect_new(void);

void rect_free(Rect *self);

float rect_get_x(Rect *self);

void rect_set_x(Rect *self, float value);

float rect_get_y(Rect *self);

void rect_set_y(Rect *self, float value);


#endif /* RECT_H */
