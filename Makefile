P=wat

#HEADERS=wat.h tinymt32.h
#SOURCES=wat.c tinymt32.c
OBJECTS=\
		wat_dod.o           \
		wat.o               \
		tinymt32.o          \
		util.o              \
		text.o              \
		config.o            \
		bullet_manager.o    \
		player_manager.o    \
		enemy_manager.o     \
		particle.o          \
		particle_manager.o  \
		score.o             \
		collision_manager.o \
		hud.o               \
		welcome_state.o     \
		in_game_state.o     \
		pause_state.o       \
		game.o              \
		input_manager.o

CFLAGS=-Wall -O3 `sdl2-config --cflags`
#CFLAGS=-Wall -g -O0 `sdl2-config --cflags` # CFLAGS for debug

LDLIBS=-lm `sdl2-config --libs`

CC=gcc


$(P): $(OBJECTS)

.PHONY: clean
clean:
	rm -f *.o $(P)
