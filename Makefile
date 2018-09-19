P=wat
SOURCES=wat.c
OBJECTS=wat.o
HEADERS=wat.h
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
