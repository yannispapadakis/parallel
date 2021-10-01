CC = gcc
CFLAGS = -Wall -Wextra -g -O3 -std=c99
LDFLAGS = -lm

all: colorgraph

colorgraph: jones.o graph.o
	$(CC) $(CFLAGS) -o colorgraph jones.o graph.o $(LDFLAGS)

jones.o: jones.c
	$(CC) $(CFLAGS) -c -o jones.o jones.c

graph.o:
	$(CC) $(CFLAGS) -c -o graph.o graph.c $(LDFLAGS)

clean:
	rm -f *.o colorgraph
