#include "actor.h"
#include "sdl_rect.h"

Actor *actor_new(void)
{
    Actor *out = malloc(sizeof(Actor));

    SDL_Rect *rect = sdl_rect_new();
    out->rect = rect;

    return out;
}

void actor_free(Actor *self)
{
    sdl_rect_free(self->rect);

    free(self);
}

float actor_get_x(Actor *self)
{
    return self->x;
}

void actor_set_x(Actor *self, float value)
{
    if (value < 0) {
        value = 0;
    } else if (value > WINDOW_WIDTH) {
        value = WINDOW_WIDTH;
    }

    self->x = value;

    self->rect->x = floor(self->x - (self->width / 2));
}

float actor_get_y(Actor *self)
{
    return self->y;
}

void actor_set_y(Actor *self, float value)
{
    if (value < 0) {
        value = 0;
    } else if (value > WINDOW_HEIGHT) {
        value = WINDOW_HEIGHT;
    }

    self->y = value;

    self->rect->y = floor(self->y - (self->height / 2));
}

int actor_get_width(Actor *self)
{
    return self->width;
}

void actor_set_width(Actor *self, int value)
{
    self->rect->w = value;
    self->width = value;
}

int actor_get_height(Actor *self)
{
    return self->height;
}

void actor_set_height(Actor *self, int value)
{
    self->rect->h = value;
    self->height = value;
}

int actor_get_velocity(Actor *self)
{
    return self->velocity;
}

void actor_set_velocity(Actor *self, int value)
{
    self->velocity = value;
}

int actor_get_angle(Actor *self)
{
    return self->angle;
}

void actor_set_angle(Actor *self, int value)
{
    self->angle = value;
}

int actor_render(Actor *self, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int tmp = SDL_RenderFillRect(renderer, self->rect);

    if (tmp == 0) {
        return 0;
    }

    return -1;
}
