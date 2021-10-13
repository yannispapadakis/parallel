CC = gcc
CFLAGS = -Wall -Wextra -g -O3 -std=c99 -D_POSIX_C_SOURCE=199309L
LDFLAGS = -lm

all: graphcolor_serial object_clean

graphcolor_serial: serial_impl.o graph.o serial_coloring.o
	$(CC) $(CFLAGS) -o graphcolor_serial serial_impl.o graph.o serial_coloring.o $(LDFLAGS)

serial_coloring.o: serial_coloring.c
	$(CC) $(CFLAGS) -c -o serial_coloring.o serial_coloring.c

serial_impl.o: serial_impl.c
	$(CC) $(CFLAGS) -c -o serial_impl.o serial_impl.c

graph.o:
	$(CC) $(CFLAGS) -c -o graph.o graph.c $(LDFLAGS)

object_clean:
	rm -f *.o

clean:
	rm -f *.o graphcolor_serial
