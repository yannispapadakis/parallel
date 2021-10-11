CC = gcc
CFLAGS = -Wall -Wextra -g -O3 -std=c99
LDFLAGS = -lm

all: naive colorgraph ldf

colorgraph: jones.o graph.o
	$(CC) $(CFLAGS) -o colorgraph jones.o graph.o $(LDFLAGS)

naive: naive.o graph.o
	$(CC) $(CFLAGS) -o naivegraph naive.o graph.o $(LDFLAGS)

ldf: ldf.o graph.o
	$(CC) $(CFLAGS) -o ldfgraph ldf.o graph.o $(LDFLAGS)

jones.o: jones.c
	$(CC) $(CFLAGS) -c -o jones.o jones.c

naive.o: naive.c
	$(CC) $(CFLAGS) -c -o naive.o naive.c

ldf.o: ldf.c
	$(CC) $(CFLAGS) -c -o ldf.o ldf.c

graph.o:
	$(CC) $(CFLAGS) -c -o graph.o graph.c $(LDFLAGS)

clean:
	rm -f *.o colorgraph naivegraph ldfgraph
