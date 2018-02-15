#include "actor.h"

UT_icd sdl_rect_icd = { sizeof(SDL_Rect), NULL, NULL, NULL };

Actor *actor_new(void)
{
    Actor *out = malloc(sizeof(Actor));

    utarray_new(out->rects, &sdl_rect_icd);

    return out;
}

void actor_free(Actor *self)
{
    utarray_free(self->rects);

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

    SDL_Rect *p;

    for(
        p = (SDL_Rect*)utarray_front(self->rects);
        p != NULL;
        p = (SDL_Rect*)utarray_next(self->rects, p)
    ) {
        p->x = floor(self->x - (self->width / 2));
    }
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

    SDL_Rect *p;

    for(
        p = (SDL_Rect*)utarray_front(self->rects);
        p != NULL;
        p = (SDL_Rect*)utarray_next(self->rects, p)
    ) {
        p->y = floor(self->y - (self->width / 2));
    }
}

int actor_get_width(Actor *self)
{
    return self->width;
}

void actor_set_width(Actor *self, int value)
{
    self->width = value;

    SDL_Rect *p;

    for(
        p = (SDL_Rect*)utarray_front(self->rects);
        p != NULL;
        p = (SDL_Rect*)utarray_next(self->rects, p)
    ) {
        p->w = value;
    }
}

int actor_get_height(Actor *self)
{
    return self->height;
}

void actor_set_height(Actor *self, int value)
{
    self->height = value;

    SDL_Rect *p;

    for(
        p = (SDL_Rect*)utarray_front(self->rects);
        p != NULL;
        p = (SDL_Rect*)utarray_next(self->rects, p)
    ) {
        p->h = value;
    }
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

void actor_add_rect(Actor *self)
{
    SDL_Rect *rect = sdl_rect_new();

    utarray_push_back(self->rects, &rect);
}

int actor_render(Actor *self, SDL_Renderer *renderer)
{
    SDL_Rect *p;

    for(
        p = (SDL_Rect*)utarray_front(self->rects);
        p != NULL;
        p = (SDL_Rect*)utarray_next(self->rects, p)
    ) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        int tmp = SDL_RenderFillRect(renderer, p);

        if (tmp != 0) {
            return -1;
        }
    }

    return 0;
}
