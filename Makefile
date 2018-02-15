P=wat
SOURCES=wat.c sdl_rect.c background.c actor.c game.c
OBJECTS=wat.o sdl_rect.o background.o actor.o game.o
HEADERS=wat.h sdl_rect.h background.h actor.h game.h
OUT=wat
CFLAGS=-Wall -O3 `sdl2-config --cflags`
#CFLAGS=-Wall -g -O0 `sdl2-config --cflags` # debugging CFLAGS
LDLIBS=-lm `sdl2-config --libs`
CC=gcc
RM=rm -f

.PHONY: clean

# default goal

all: $(P)

$(P): $(OBJECTS)

clean:
	rm -f *.o $(P)
