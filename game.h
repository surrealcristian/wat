#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "actor.h"

typedef struct Game {

} Game;

Game *game_new(void);

void game_free(Game *self);

void game_run(Game *game, SDL_Renderer *renderer);

#endif /* GAME_H */
