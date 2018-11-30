#ifndef TEXT_H
#define TEXT_H

#include "SDL.h"

#define TEXT_SIZE_SMALL  0
#define TEXT_SIZE_MEDIUM 1
#define TEXT_SIZE_LARGE  2

#define TEXT_ALIGN_LEFT   0
#define TEXT_ALIGN_CENTER 1
#define TEXT_ALIGN_RIGHT  2

#define RUNE_SMALL_PX  2
#define RUNE_MEDIUM_PX 4
#define RUNE_LARGE_PX  8

#define RUNE_W       5
#define RUNE_H       5
#define RUNE_SPACE_W 1

struct Text {
    SDL_Rect rect;
    char     *value;
    int      size;
    int      align;
    float    x;
    float    y;

    int      value_len;
    int      size_px;
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

void text_render_rune(
    struct Text  *self,
    SDL_Renderer *renderer,
    int          rune,
    float        x,
    float        y
);

void text_render(
    struct Text  *self,
    SDL_Renderer *renderer
);

#endif
