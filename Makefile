P=wat

#HEADERS=wat.h tinymt32.h
#SOURCES=wat.c tinymt32.c
OBJECTS=tinymt32.o wat.o

CFLAGS=-Wall -O3 `sdl2-config --cflags`
#CFLAGS=-Wall -g -O0 `sdl2-config --cflags` # CFLAGS for debug

LDLIBS=-lm `sdl2-config --libs`

CC=gcc


$(P): $(OBJECTS)

.PHONY: clean
clean:
	rm -f *.o $(P)
