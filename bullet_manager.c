#include "bullet_manager.h"

void bullet_manager_init(
    struct BulletManager *self,
    struct Bullet        *bullets,
    int                  n,
    int                  w,
    int                  h,
    int                  v
) {
    self->bullets = bullets;
    self->n = n;

    for (int i = 0; i < n; i++) {
        bullet_init(&bullets[i], 0, 0, w, h, v);
    }
}

struct Bullet *bullet_manager_get_free(
    struct BulletManager *self
) {
    struct Bullet *bullet = NULL;

    for (int i = 0; i < self->n; i++) {
        if (self->bullets[i].alive == 1) {
            continue;
        }

        bullet = &self->bullets[i];

        break;
    }

    return bullet;
}

void bullet_manager_update(
    struct BulletManager *self
) {
    for (int i = 0; i < self->n; i++) {
        if (self->bullets[i].alive == 0) {
            continue;
        }

        bullet_update(&self->bullets[i]);
    }
}

void bullet_manager_render(
    struct BulletManager *self,
    SDL_Renderer         *renderer
) {
    for (int i = 0; i < self->n; i++) {
        if (self->bullets[i].alive == 0) {
            continue;
        }

        bullet_render(&self->bullets[i], renderer);
    }
}
