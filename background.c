#include "background.h"

Background *background_new(void)
{
    Background *out = malloc(sizeof(Background));

    return out;
}

void background_free(Background *self)
{
    free(self);
}

int background_render(Background *self, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // grey
    int tmp = SDL_RenderClear(renderer);

    if (tmp == 0) {
        return 0;
    }

    return -1;
}
