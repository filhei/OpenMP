CC=gcc
STD=-std=c11
OFLAGS=-O3 -ffast-math
LIBFLAGS=-lm -lgomp

.PHONY: all clean bench

all: cell_distance

cell_distance:cell_distance.c
	$(CC) -fopenmp $(OFLAGS) $(STD) -o $@ $< $(LIBFLAGS)

clean:
	rm cell_distance
