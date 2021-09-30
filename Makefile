CC = gcc
CFLAGS = -Wall -Wextra -g -O3 -std=c99
LDFLAGS = -lm

graph: 
	$(CC) $(CFLAGS) -o par graph.c $(LDFLAGS)

clean:
	rm par
