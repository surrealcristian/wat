#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "SDL.h"

typedef struct Background {

} Background;

Background *background_new(void);

void background_free(Background *self);

int background_render(Background *self, SDL_Renderer *renderer);

#endif /* BACKGROUND_H */
