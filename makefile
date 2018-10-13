CC=gcc
STD=-std=c99
OFLAGS=-O2 -ffast-math
LIBFLAGS=-lm -lgomp

.PHONY: all clean

all: cell_distance cell_debug

cell_distance:cell_distance.c
	$(CC) -fopenmp $(OFLAGS) $(STD) -o $@ $< $(LIBFLAGS)

#cell_debug:cell_distance.c
#	$(CC) $(STD) -g -o $@ $< $(LIBFLAGS)

clean:
	rm cell_distance
	rm cell_debug
