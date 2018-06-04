CC=gcc
CFLAGS=-I. -Os
DEPS = datasheet.h hw.h
OBJ = eps.o hw.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

eps-monolith.o: eps.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -D MONOLITHIC


eps: eps.o hw.o
	$(CC) -o $@ $^ $(CFLAGS)

eps-sim: eps.o hw_sim.o
	$(CC) -o $@ $^ $(CFLAGS)

eps-monolith: eps-monolith.o
	$(CC) -o $@ $^ $(CFLAGS)

