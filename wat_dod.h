#ifndef WAT_DOD_H
#define WAT_DOD_H

#define PLAYER_MAX                   1
#define BULLET_PER_PLAYER            128
#define BULLET_MAX                   PLAYER_MAX * BULLET_PER_PLAYER
#define ENEMY_MAX                    16
#define ENTITY_MAX                   PLAYER_MAX + BULLET_MAX + ENEMY_MAX

#define PHYSICS_SDL_RECT_PER_PLAYER  1
#define PHYSICS_SDL_RECT_PER_BULLET  1
#define PHYSICS_SDL_RECT_PER_ENEMY   1
#define PHYSICS_MAX                  PLAYER_MAX + BULLET_MAX + ENEMY_MAX
#define PHYSICS_SDL_RECT_MAX         (PLAYER_MAX * PHYSICS_SDL_RECT_PER_PLAYER) + (BULLET_MAX * PHYSICS_SDL_RECT_PER_BULLET) + (ENEMY_MAX + PHYSICS_SDL_RECT_PER_ENEMY)
#define PHYSICS_SDL_RECT_PER_ENTITY  1

#define GRAPHICS_SDL_RECT_PER_PLAYER 1
#define GRAPHICS_SDL_RECT_PER_BULLET 1
#define GRAPHICS_SDL_RECT_PER_ENEMY  1
#define GRAPHICS_MAX                 PLAYER_MAX + BULLET_MAX + ENEMY_MAX
#define GRAPHICS_SDL_RECT_MAX        (PLAYER_MAX * GRAPHICS_SDL_RECT_PER_PLAYER) + (BULLET_MAX * GRAPHICS_SDL_RECT_PER_BULLET) + (ENEMY_MAX * GRAPHICS_SDL_RECT_PER_ENEMY)
#define GRAPHICS_SDL_RECT_PER_ENTITY 1

#define SHOOTING_MAX                 PLAYER_MAX


// PhysicsComponent

struct PhysicsComponent {
    float x;
    float y;
    int   w;
    int   h;
    int   v;
    int   vx;
    int   vy;
    int   alive;
};

// GraphicsComponent

struct GraphicsComponent {
    int id;
    float x;
    float y;
    int w;
    int h;
};

// ShootingComponent

struct ShootingComponent {
    int   bullets_n;
    int   firing;
    int   fire_spacing;
    float fire_time; 
};

// Entity

struct Entity {
    int id;
};

struct EntityPhysicsComponent {
    int entity_id;
    int physics_id;
};

struct EntityShootingComponent {
    int entity_id;
    int shooting_id;
};

#endif
