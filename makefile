P=wat
OBJECTS=
CFLAGS=-g -Wall -O3 `sdl2-config --cflags`
LDLIBS=-lm `sdl2-config --libs`
CC=gcc

$(P): $(OBJECTS)
