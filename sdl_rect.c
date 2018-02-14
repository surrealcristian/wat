#include "sdl_rect.h"

SDL_Rect *sdl_rect_new(void)
{
    SDL_Rect *out = malloc(sizeof(SDL_Rect));

    return out;
}

void sdl_rect_free(SDL_Rect *self)
{
    free(self);
}
