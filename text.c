#include "text.h"

float RUNE_X_POSITIONS[25] = { 0, 1, 2, 3, 4,    0, 1, 2, 3, 4,    0, 1, 2, 3, 4,    0, 1, 2, 3, 4,    0, 1, 2, 3, 4, };
float RUNE_Y_POSITIONS[25] = { 0, 0, 0, 0, 0,    1, 1, 1, 1, 1,    2, 2, 2, 2, 2,    3, 3, 3, 3, 3,    4, 4, 4, 4, 4, };

int RUNES[36][25] = {
    /* 0 */ { 0, 1, 1, 1, 0,    1, 0, 0, 1, 1,    1, 0, 1, 0, 1,    1, 1, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* 1 */ { 0, 0, 1, 0, 0,    1, 1, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    1, 1, 1, 1, 1, },
    /* 2 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 0, 1, 1, 0,    0, 1, 0, 0, 0,    1, 1, 1, 1, 1, },
    /* 3 */ { 1, 1, 1, 1, 0,    0, 0, 0, 0, 1,    0, 0, 1, 1, 0,    0, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* 4 */ { 0, 0, 0, 1, 0,    0, 0, 1, 1, 0,    1, 1, 0, 1, 0,    1, 1, 1, 1, 1,    0, 0, 0, 1, 0, },
    /* 5 */ { 1, 1, 1, 1, 1,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    0, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* 6 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* 7 */ { 1, 1, 1, 1, 1,    0, 0, 0, 0, 1,    0, 0, 0, 1, 0,    0, 0, 1, 0, 0,    0, 1, 0, 0, 0, },
    /* 8 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* 9 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 1,    0, 0, 0, 0, 1,    0, 1, 1, 1, 0, },

    /* A */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1, },
    /* B */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* C */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 0, 0, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* D */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* E */ { 1, 1, 1, 1, 1,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 1, 1, 1, 1, },
    /* F */ { 1, 1, 1, 1, 1,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0, },
    /* G */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 0, 0, 1, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* H */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 1, 1, 1, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1, },
    /* I */ { 1, 1, 1, 1, 1,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    1, 1, 1, 1, 1, },
    /* J */ { 0, 1, 1, 1, 1,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },

    /* K */ { 1, 0, 0, 0, 1,    1, 0, 0, 1, 0,    1, 1, 1, 0, 0,    1, 0, 0, 1, 0,    1, 0, 0, 0, 1, },
    /* L */ { 1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    1, 1, 1, 1, 1, },
    /* M */ { 1, 0, 0, 0, 1,    1, 1, 0, 1, 1,    1, 0, 1, 0, 1,    1, 0, 1, 0, 1,    1, 0, 0, 0, 1, },
    /* N */ { 1, 0, 0, 0, 1,    1, 1, 0, 0, 1,    1, 0, 1, 0, 1,    1, 0, 0, 1, 1,    1, 0, 0, 0, 1, },
    /* O */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* P */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0, },
    /* Q */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 1, 0, 1,    1, 0, 0, 1, 0,    0, 1, 1, 0, 1, },
    /* R */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0,    1, 0, 0, 1, 0,    1, 0, 0, 0, 1, },
    /* S */ { 0, 1, 1, 1, 1,    1, 0, 0, 0, 0,    0, 1, 1, 1, 0,    0, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* T */ { 1, 1, 1, 1, 1,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0, },

    /* U */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* V */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 0, 1, 0,    0, 0, 1, 0, 0, },
    /* W */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 1, 0, 1,    1, 1, 0, 1, 1,    1, 0, 0, 0, 1, },
    /* X */ { 1, 0, 0, 0, 1,    0, 1, 0, 1, 0,    0, 0, 1, 0, 0,    0, 1, 0, 1, 0,    1, 0, 0, 0, 1, },
    /* Y */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 0, 1, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0, },
    /* Z */ { 1, 1, 1, 1, 1,    0, 0, 0, 1, 0,    0, 0, 1, 0, 0,    0, 1, 0, 0, 0,    1, 1, 1, 1, 1, },
};

void text_init(
    struct Text *self,
    char        *value,
    int         size,
    int         align,
    float       x,
    float       y
) {
    self->value = value;
    self->size  = size;
    self->align = align;
    self->x     = x;
    self->y     = y;

    if (self->size == TEXT_SIZE_SMALL) {
        self->size_px = RUNE_SMALL_PX;
    } else if (self->size == TEXT_SIZE_MEDIUM) {
        self->size_px = RUNE_MEDIUM_PX;
    } else if (self->size == TEXT_SIZE_LARGE) {
        self->size_px = RUNE_LARGE_PX;
    }

    self->rect.w = self->size_px;
    self->rect.h = self->size_px;

    self->value_len = strlen(self->value);
}

void text_set_value(
    struct Text *self,
    char        *value
) {
    self->value = value;
    self->value_len = strlen(value);
}

void text_set_size(
    struct Text *self,
    int         size
) {
    self->size = size;
}

void text_set_align(
    struct Text *self,
    int         align
) {
    self->align = align;
}

void text_set_x(
    struct Text *self,
    float       x
) {
    self->x = x;
}

void text_set_y(
    struct Text *self,
    float       y
) {
    self->y = y;
}

void text_render_rune(
    struct Text  *self,
    SDL_Renderer *renderer,
    int          rune,
    float        x,
    float        y
) {
    for (int i = 0; i < 25; i++) {
        if (RUNES[rune][i] == 0) {
            continue;
        }

        self->rect.x = x + RUNE_X_POSITIONS[i] * self->size_px;
        self->rect.y = y + RUNE_Y_POSITIONS[i] * self->size_px;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &self->rect);
    }
}

void text_render(
    struct Text  *self,
    SDL_Renderer *renderer
) {
    int   rune_w_px       = RUNE_W * self->size_px;
    int   rune_h_px       = RUNE_H * self->size_px;
    int   rune_space_w_px = RUNE_SPACE_W * self->size_px;
    float text_w_px       = (self->value_len * rune_w_px) + ((self->value_len - 1) * rune_space_w_px);
    float rune_x          = 0;
    float rune_y          = 0;

    if (self->align == TEXT_ALIGN_LEFT) {
        rune_x = self->x;
    } else if (self->align == TEXT_ALIGN_CENTER) {
        rune_x = self->x - (text_w_px / 2);
    } else if (self->align == TEXT_ALIGN_RIGHT) {
        rune_x = self->x - text_w_px;
    }

    rune_y = self->y - (rune_h_px / 2);

    int idx;

    for (int i = 0; self->value[i] != '\0'; i++) {
        int c = self->value[i];

        if (c >= '0' && c <= '9') {
            idx = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            idx = c - 'A' + 10;
        } else {
            return;
        }

        text_render_rune(self, renderer, idx, rune_x, rune_y);

        rune_x += rune_w_px + rune_space_w_px;
    }
}
