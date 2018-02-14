#ifndef SDL_RECT_H
#define SDL_RECT_H

#include "SDL.h"

SDL_Rect *sdl_rect_new(void);
void sdl_rect_free(SDL_Rect *self);

#endif /* SDL_RECT_H */
