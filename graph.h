#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct AdjListNode {
  int dest;
};

// A structure to represent an adjacency list - equivalent to a Vertex
struct AdjList {
  unsigned int degree;
  unsigned int VertexID;
  int weight;
  struct AdjListNode* neighbor;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph {
  unsigned int V;
  unsigned int edges;
  unsigned int maxDegree;
  struct AdjList* vertex;
};

struct Graph* graph_read(const char* filename);
void printcolors(int* colors, unsigned int V);
void printerrors(struct Graph* graph, int* colors);
void find_min_max(int* colors, unsigned int V);
void first_available_color(struct Graph* graph, bool* is_available, int* colors,
                           unsigned int i);
