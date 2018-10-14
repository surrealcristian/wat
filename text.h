#ifndef TEXT_H
#define TEXT_H

#include "SDL.h"

struct Text {
    SDL_Rect rect;
};

void text_init(struct Text *text);
void text_render_character(struct Text *text, SDL_Renderer *renderer, int character, float x, float y);
void text_render(struct Text *self, SDL_Renderer *renderer, char *text, float x, float y);

#endif
