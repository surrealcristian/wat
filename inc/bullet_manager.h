#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "SDL.h"
#include "bullet.h"

struct BulletManager {
    struct Bullet *bullets;
    int n;
};

void bullet_manager_init(struct BulletManager *self, struct Bullet *bullets, int n, int w, int h, int v);
struct Bullet *bullet_manager_get_free(struct BulletManager *self);
void bullet_manager_update(struct BulletManager *self);
void bullet_manager_render(struct BulletManager *self, SDL_Renderer *renderer);

#endif
