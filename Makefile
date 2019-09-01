P=wat

#HEADERS=wat.h tinymt32.h
#SOURCES=wat.c tinymt32.c
OBJECTS=\
		wat.o \
		xoshiro128.o \
		util.o \
		text.o \
		config.o \
		bullet.o \
		bullet_manager.o \
		player.o \
		enemy.o \
		enemy_manager.o \
		particle.o \
		particle_manager.o \
		collision_manager.o \
		in_game_state.o \
		pause_state.o \
		game.o \
		input_component.o

CFLAGS=-Wall -O3 `sdl2-config --cflags`
#CFLAGS=-Wall -g -O0 `sdl2-config --cflags` # CFLAGS for debug

LDLIBS=-lm `sdl2-config --libs`

CC=gcc

.PHONY: clean

# default goal

all: $(P)

$(P): $(OBJECTS)

clean:
	rm -f *.o $(P)
