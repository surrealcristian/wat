P=wat
SOURCES=wat.c sdl_rect.c actor.c game.c
OBJECTS=wat.o sdl_rect.o actor.o game.o
HEADERS=wat.h sdl_rect.h utarray.h actor.h utlist.h game.h
OUT=wat
CFLAGS=-Wall -O3 `sdl2-config --cflags`
#CFLAGS=-Wall -g -O0 `sdl2-config --cflags` # CFLAGS for debug
LDLIBS=-lm `sdl2-config --libs`
CC=gcc
RM=rm -f

.PHONY: clean

# default goal

all: $(P)

$(P): $(OBJECTS)

clean:
	rm -f *.o $(P)
