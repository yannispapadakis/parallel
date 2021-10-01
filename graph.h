#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct AdjListNode {
	int dest;
};

// A structure to represent an adjacency list
struct AdjList {
	unsigned int neighbors;
	struct AdjListNode *head;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph {
	unsigned int V;
	unsigned int edges;
	unsigned int maxDegree;
	struct AdjList* array;
};

struct Graph* graph_read(const char *filename);
