#ifndef TEXT_H
#define TEXT_H

#include "SDL.h"

#define TEXT_SMALL  0
#define TEXT_MEDIUM 1
#define TEXT_LARGE  2

#define TEXT_SMALL_PX  2
#define TEXT_MEDIUM_PX 4
#define TEXT_LARGE_PX  8

#define TEXT_LEFT   0
#define TEXT_CENTER 1
#define TEXT_RIGHT  2

#define CHARACTER_N_SQUARES         5
#define CHARACTER_SPACING_N_SQUARES 1

struct Text {
    SDL_Rect rect;
    char     *value;
    int      size;
    int      align;
    float    x;
    float    y;

    int      value_len;
    int      size_px;
    float    init_x;
    float    init_y;
};

void text_init(
    struct Text *text,
    char        *value,
    int         size,
    int         align,
    float       x,
    float       y
);

void text_set_value(
    struct Text *text,
    char        *value
);

void text_set_size(
    struct Text *text,
    int         size
);

void text_set_align(
    struct Text *text,
    int         align
);

void text_set_x(
    struct Text *text,
    float       x
);

void text_set_y(
    struct Text *text,
    float       y
);

void text_render_character(
    struct Text  *self,
    SDL_Renderer *renderer,
    int          character,
    float        x,
    float        y
);

void text_render(
    struct Text  *self,
    SDL_Renderer *renderer
);

#endif