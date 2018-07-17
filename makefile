CC=gcc
CFLAGS=-I. -Os
DEPS = datasheet.h hw.h
OBJ = eps.o hw.o 
BIN = bin/

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

eps-monolith.o: eps.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -D MONOLITHIC


eps: eps.o hw.o
	$(CC) -o $(BIN)$@ $^ $(CFLAGS)

eps-sim: eps.o hw_sim.o
	$(CC) -o $(BIN)$@ $^ $(CFLAGS)

eps-monolith: eps-monolith.o
	$(CC) -o $(BIN)$@ $^ $(CFLAGS)

