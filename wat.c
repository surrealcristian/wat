#include <stdio.h>
#include <math.h>
#include "SDL.h"

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480


// # Structs

// ## SDL_Rect

SDL_Rect *sdl_rect_new(void)
{
    SDL_Rect *out = malloc(sizeof(SDL_Rect));
    *out = (SDL_Rect){ };

    return out;
}

void sdl_rect_free(SDL_Rect *self)
{
    free(self);
}

// ## Sprite

typedef struct Sprite {
    float x;
    float y;
    int width;
    int height;
    int velocity;
    int angle;
    SDL_Rect *rect;
} Sprite;

Sprite *sprite_new(void)
{
    Sprite *out = malloc(sizeof(Sprite));
    *out = (Sprite){ };

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

// ## Background

typedef struct Background {

} Background;

Background *background_new(void)
{
    Background *out = malloc(sizeof(Background));
    *out = (Background){ };

    return out;
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


// # Main function

int main(void)
{
    // [init SDL]

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("ERROR: SDL_Init() (%s)", SDL_GetError());
        return 1;
    }

    SDL_Log("INFO: SDL_Init()");

    // [create window]

    SDL_Window *window = SDL_CreateWindow(
        "SHMUP",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if (window == NULL) {
        SDL_Log("ERROR: SDL_CreateWindow() (%s)", SDL_GetError());

        SDL_Quit();
        SDL_Log("INFO: SDL_Quit()");

        return 1;
    }

    SDL_Log("INFO: SDL_CreateWindow()");

    // [create renderer]

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL) {
        SDL_Log("ERROR: SDL_CreateRenderer() (%s)", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Log("INFO: SDL_DestroyWindow()");

        SDL_Quit();
        SDL_Log("INFO: SDL_Quit()");

        return 1;
    }

    SDL_Log("INFO: SDL_CreateRenderer()");

    Background *background = background_new();
    Sprite *sprite1 = sprite_new();

    background_render(background, renderer);
    SDL_RenderPresent(renderer);

    sprite_set_width(sprite1, 32);
    sprite_set_height(sprite1, 32);
    sprite_set_velocity(sprite1, 7);
    sprite_set_angle(sprite1, 7);

    sprite_set_x(sprite1, 300);
    sprite_set_y(sprite1, 300);

    background_render(background, renderer);
    sprite_render(sprite1, renderer);
    SDL_RenderPresent(renderer);

    // -----

    SDL_Delay(1000);

    // -----

    sprite_set_x(sprite1, 200);
    sprite_set_y(sprite1, 200);

    background_render(background, renderer);
    sprite_render(sprite1, renderer);
    SDL_RenderPresent(renderer);

    // -----

    SDL_Delay(1000);

    // -----

    sprite_set_x(sprite1, 1000);
    sprite_set_y(sprite1, 1000);

    background_render(background, renderer);
    sprite_render(sprite1, renderer);
    SDL_RenderPresent(renderer);

    while (1) {
        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
        }

        if (event.type == SDL_KEYDOWN) {
            float prev_x;

            switch (event.key.keysym.sym) {

            case SDLK_LEFT:
                prev_x = sprite_get_x(sprite1);
                sprite_set_x(sprite1, prev_x - 10);
                break;

            case SDLK_RIGHT:
                prev_x = sprite_get_x(sprite1);
                sprite_set_x(sprite1, prev_x + 10);
                break;

            }
        }

        background_render(background, renderer);
        sprite_render(sprite1, renderer);
        SDL_RenderPresent(renderer);
    }

    sprite_free(sprite1);

    // [destroy renderer]

    SDL_DestroyRenderer(renderer);
    SDL_Log("INFO: SDL_DestroyRenderer()");

    // [destroy window]

    SDL_DestroyWindow(window);
    SDL_Log("INFO: SDL_DestroyWindow()");

    // [quit SDL]

    SDL_Quit();
    SDL_Log("INFO: SDL_Quit()");

    return 0;
}
