P=wat
SOURCES=wat.c sdl_rect.c background.c sprite.c game.c
OBJECTS=wat.o sdl_rect.o background.o sprite.o game.o
HEADERS=wat.h sdl_rect.h background.h sprite.h game.h
OUT=wat
CFLAGS=-g -Wall -O3 `sdl2-config --cflags`
LDLIBS=-lm `sdl2-config --libs`
CC=gcc
RM=rm -f

.PHONY: clean

# default goal

all: $(P)

$(P): $(OBJECTS)

clean:
	rm -f *.o $(P)
