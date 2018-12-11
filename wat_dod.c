#define PLAYER_MAX                             1
#define BULLET_PER_PLAYER                      128
#define BULLET_MAX                             PLAYER_MAX * BULLET_PER_PLAYER
#define ENEMY_MAX                              16
#define ENTITY_MAX                             PLAYER_MAX + BULLET_MAX + ENEMY_MAX

#define PHYSICS_COMPONENT_SDL_RECT_PER_PLAYER  1
#define PHYSICS_COMPONENT_SDL_RECT_PER_BULLET  1
#define PHYSICS_COMPONENT_SDL_RECT_PER_ENEMY   1
#define PHYSICS_COMPONENT_MAX                  PLAYER_MAX + BULLET_MAX + ENEMY_MAX
#define PHYSICS_COMPONENT_SDL_RECT_MAX         (PLAYER_MAX * PHYSICS_COMPONENT_SDL_RECT_PER_PLAYER) + (BULLET_MAX * PHYSICS_COMPONENT_SDL_RECT_PER_BULLET) + (ENEMY_MAX + PHYSICS_COMPONENT_SDL_RECT_PER_ENEMY)
#define PHYSICS_COMPONENT_SDL_RECT_PER_ENTITY  1

#define GRAPHICS_COMPONENT_SDL_RECT_PER_PLAYER 1
#define GRAPHICS_COMPONENT_SDL_RECT_PER_BULLET 1
#define GRAPHICS_COMPONENT_SDL_RECT_PER_ENEMY  1
#define GRAPHICS_COMPONENT_MAX                 PLAYER_MAX + BULLET_MAX + ENEMY_MAX
#define GRAPHICS_COMPONENT_SDL_RECT_MAX        (PLAYER_MAX * GRAPHICS_COMPONENT_SDL_RECT_PER_PLAYER) + (BULLET_MAX * GRAPHICS_COMPONENT_SDL_RECT_PER_BULLET) + (ENEMY_MAX * GRAPHICS_COMPONENT_SDL_RECT_PER_ENEMY)
#define GRAPHICS_COMPONENT_SDL_RECT_PER_ENTITY 1

#define SHOOTING_COMPONENT_MAX                 PLAYER_MAX

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
    int physics_component_id;
};

struct EntityShootingComponent {
    int entity_id;
    int shooting_component_id;
};


struct PhysicsComponent PHYSICS_COMPONENTS[PHYSICS_COMPONENT_MAX];
struct SDL_Rect         PHYSICS_COMPONENT_SDL_RECTS[PHYSICS_COMPONENT_SDL_RECT_MAX];
int                     PHYSICS_COMPONENT_SDL_RECT_INDEX[PHYSICS_COMPONENT_MAX];

struct GraphicsComponent GRAPHICS_COMPONENTS[GRAPHICS_COMPONENT_MAX];
struct SDL_Rect          GRAPHICS_COMPONENT_SDL_RECTS[GRAPHICS_COMPONENT_SDL_RECT_MAX];
int                      GRAPHICS_COMPONENT_SDL_RECT_INDEX[GRAPHICS_COMPONENT_MAX];

struct ShootingComponent SHOOTING_COMPONENTS[SHOOTING_COMPONENT_MAX];

struct Entity ENTITIES[ENTITY_MAX];

int ENTITY_PHYSICS_COMPONENT_INDEX[ENTITY_MAX];
int ENTITY_GRAPHICS_COMPONENT_INDEX[ENTITY_MAX];
int ENTITY_SHOOTING_COMPONENT_INDEX[ENTITY_MAX];

int PLAYER_ENTITY_INDEX[PLAYER_MAX];
int ENEMY_ENTITY_INDEX[ENEMY_MAX];
int BULLET_ENTITY_INDEX[BULLET_MAX];

int PLAYER_BULLET_INDEX[PLAYER_MAX][BULLET_PER_PLAYER];


void init_physics_component_sdl_rect_index() {
    int physics_component_sdl_rect_i = 0;

    for (int i = 0; i < PHYSICS_COMPONENT_MAX; i++) {
        PHYSICS_COMPONENT_SDL_RECT_INDEX[i] = physics_component_sdl_rect_i;

        physics_component_sdl_rect_i++;
    }
}

void init_graphics_component_sdl_rect_index() {
    int graphics_component_sdl_rect_i = 0;

    for (int i = 0; i < GRAPHICS_COMPONENT_MAX; i++) {
        GRAPHICS_COMPONENT_SDL_RECT_INDEX[i] = graphics_component_sdl_rect_i;

        graphics_component_sdl_rect_i++;
    }
}

int main(void) {
    init_physics_component_sdl_rect_index();
    init_graphics_component_sdl_rect_index();

    int physics_component_i             = 0;
    int graphics_component_i            = 0;
    int shooting_component_i            = 0;
    int entity_i                        = 0;

    for (int i = 0; i < PLAYER_MAX; i++) {
        PLAYER_ENTITY_INDEX[i] = entity_i;

        ENTITY_PHYSICS_COMPONENT_INDEX[entity_i]  = physics_component_i;
        ENTITY_GRAPHICS_COMPONENT_INDEX[entity_i] = graphics_component_i;
        ENTITY_SHOOTING_COMPONENT_INDEX[entity_i] = shooting_component_i;
        
        entity++;
        physics_component_i++;
        graphics_component_i++;
        shooting_component_i++;
    }

    for (int i = 0; i < ENEMY_MAX; i++) {
        ENEMY_ENTITY_INDEX[i] = entity_i;

        ENTITY_PHYSICS_COMPONENT_INDEX[entity_i]  = physics_component_i;
        ENTITY_GRAPHICS_COMPONENT_INDEX[entity_i] = graphics_component_i;
        ENTITY_SHOOTING_COMPONENT_INDEX[entity_i] = shooting_component_i;

        entity++;
        physics_component_i++;
        graphics_component_i++;
        shooting_component_i++;
    }

    for (int i = 0; i < BULLET_MAX; i++) {
        BULLET_ENTITY_INDEX[i] = entity_i;

        ENTITY_PHYSICS_COMPONENT_INDEX[entity_i]  = physics_component_i;
        ENTITY_GRAPHICS_COMPONENT_INDEX[entity_i] = graphics_component_i;
        ENTITY_SHOOTING_COMPONENT_INDEX[entity_i] = shooting_component_i;
    }

    int bullet_i = 0;

    for (int i = 0; i < PLAYER_MAX; i++) {
        for (int j = 0; j < BULLET_PER_PLAYER; j++) {
            PLAYER_BULLET_INDEX[i][j] = bullet_i;

            bullet_i++;
        }
    }
};
