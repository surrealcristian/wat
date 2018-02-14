#include "sprite.h"
#include "sdl_rect.h"

Sprite *sprite_new(void)
{
    Sprite *out = malloc(sizeof(Sprite));

    SDL_Rect *rect = sdl_rect_new();
    out->rect = rect;

    return out;
}

void sprite_free(Sprite *self)
{
    sdl_rect_free(self->rect);

    free(self);
}

float sprite_get_x(Sprite *self)
{
    return self->x;
}

void sprite_set_x(Sprite *self, float value)
{
    if (value < 0) {
        value = 0;
    } else if (value > WINDOW_WIDTH) {
        value = WINDOW_WIDTH;
    }

    self->x = value;

    self->rect->x = floor(self->x - (self->width / 2));
}

float sprite_get_y(Sprite *self)
{
    return self->y;
}

void sprite_set_y(Sprite *self, float value)
{
    if (value < 0) {
        value = 0;
    } else if (value > WINDOW_HEIGHT) {
        value = WINDOW_HEIGHT;
    }

    self->y = value;

    self->rect->y = floor(self->y - (self->height / 2));
}

int sprite_get_width(Sprite *self)
{
    return self->width;
}

void sprite_set_width(Sprite *self, int value)
{
    self->rect->w = value;
    self->width = value;
}

int sprite_get_height(Sprite *self)
{
    return self->height;
}

void sprite_set_height(Sprite *self, int value)
{
    self->rect->h = value;
    self->height = value;
}

int sprite_get_velocity(Sprite *self)
{
    return self->velocity;
}

void sprite_set_velocity(Sprite *self, int value)
{
    self->velocity = value;
}

int sprite_get_angle(Sprite *self)
{
    return self->angle;
}

void sprite_set_angle(Sprite *self, int value)
{
    self->angle = value;
}

int sprite_render(Sprite *self, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int tmp = SDL_RenderFillRect(renderer, self->rect);

    if (tmp == 0) {
        return 0;
    }

    return -1;
}
