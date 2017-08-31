P=wat
OBJECTS=
CFLAGS=-g -Wall -O3 `sdl2-config --cflags`
LDLIBS=`sdl2-config --libs`
CC=gcc

$(P): $(OBJECTS)
