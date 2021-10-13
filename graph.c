#include "graph.h"

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>

// A utility function that creates a graph of V vertices
struct Graph *createGraph(int V) {
  struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
  graph->V = V;
  graph->edges = 0;
  graph->maxDegree = 0;

  // Create an array of adjacency lists.  Size of array will be V
  graph->vertex = (struct AdjList *)malloc(V * sizeof(struct AdjList));

  // Initialize each adjacency list as empty by making head as NULL
  for (int i = 0; i < V; ++i) {
    graph->vertex[i].neighbor = NULL;
    graph->vertex[i].degree = 0;
    graph->vertex[i].VertexID = i;
    graph->vertex[i].colored = false;
  }

  return graph;
}

// A utility function to create a new adjacency list node
struct AdjListNode *newAdjListNode(int dest) {
  struct AdjListNode *newNode =
      (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
  newNode->dest = dest;
  return newNode;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph *graph, int src, int dest) {
  // Add an edge from src to dest.  A new node is added to the adjacency
  // list of src.  The node is added at the begining
  struct AdjListNode *newNode = newAdjListNode(dest);
  if (graph->vertex[src].neighbor == NULL)
    graph->vertex[src].neighbor =
        (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
  else
    graph->vertex[src].neighbor = (struct AdjListNode *)realloc(
        graph->vertex[src].neighbor,
        (graph->vertex[src].degree + 1) * sizeof(struct AdjListNode));

  graph->vertex[src].neighbor[graph->vertex[src].degree] = *newNode;
  graph->vertex[src].degree++;

  // Store the maximimum vertex degree.
  if (graph->maxDegree < graph->vertex[src].degree)
    graph->maxDegree = graph->vertex[src].degree;

  // Since graph is undirected, add an edge from dest to src also
  newNode = newAdjListNode(src);

  if (graph->vertex[dest].neighbor == NULL)
    graph->vertex[dest].neighbor =
        (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
  else
    graph->vertex[dest].neighbor = (struct AdjListNode *)realloc(
        graph->vertex[dest].neighbor,
        (graph->vertex[dest].degree + 1) * sizeof(struct AdjListNode));

  graph->vertex[dest].neighbor[graph->vertex[dest].degree] = *newNode;
  graph->vertex[dest].degree++;

  // Store the maximum vertex degree.
  if (graph->maxDegree < graph->vertex[dest].degree)
    graph->maxDegree = graph->vertex[dest].degree;

  graph->edges++;
}

// Read graph
struct Graph *graph_read(const char *filename) {
  FILE *fp;
  fp = fopen(filename, "r");
  char line[50];
  char *token;
  int dest, source, V = 0, edges = 0;
  struct Graph *graph = NULL;

  while (fgets(line, sizeof line, fp) != NULL) {
    token = strtok(line, " ");

    if (strcmp(token, "p") == 0) {
      while (V == 0) V = atoi(strtok(NULL, " "));
      graph = createGraph(V);
    } else if (strcmp(token, "a") == 0 || strcmp(token, "e") == 0) {
      source = atoi(strtok(NULL, " "));
      dest = atoi(strtok(NULL, " "));
      edges++;
      addEdge(graph, source - 1, dest - 1);
    }
  }

  return graph;
}

double get_timestamp() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_usec + tv.tv_sec * 1e6;
}

void printcolors(int *colors, struct Graph *graph) {
  for (int i = 0; i < graph->V; i++)
    printf("Vertex: %d is %s colored-> color: %d\n", i,
           (graph->vertex[i].colored ? "" : "NOT"), colors[i]);
}

void printerrors(struct Graph *graph, int *colors) {
  // check if a vertice has same color with a neighbor
  for (int i = 0; i < graph->V; i++) {
    for (int j = 0; j < graph->vertex[i].degree; j++) {
      if (colors[graph->vertex[i].VertexID] ==
          colors[graph->vertex[i].neighbor[j].dest])
        printf("Error between %d and %d\n", graph->vertex[i].VertexID,
               graph->vertex[i].neighbor[j].dest);
    }
  }
}

void find_min_max(int *colors, int V) {
  int min = V + 1, max = -1;
  for (int i = 0; i < V; i++) {
    if (colors[i] < min) min = colors[i];
    if (colors[i] > max) max = colors[i];
  }
  //  printf("Min: %d, Max: %d\n", min, max);
  printf("Colors used: %d\n", max);
}

void init_weights(struct Graph *graph) {
  for (int i = 0; i < graph->V; i++)
    graph->vertex[i].weight = rand() % (graph->V * 1000);
}

void first_available_color(struct Graph *graph, bool *is_available, int *colors,
                           int i) {
  int n;
  // set the colors of all adjacent vertices as unavailable
  for (n = 0; n < graph->vertex[i].degree; n++) {
    if (colors[graph->vertex[i].neighbor[n].dest] != 0) {
      is_available[colors[graph->vertex[i].neighbor[n].dest]] = false;
    }
  }

  // find the first available color
  for (n = 1; n < graph->V; n++) {
    if (is_available[n]) {
      break;
    }
  }

  colors[i] = n;
  graph->vertex[i].colored = true;

  // reset the color availability
  for (n = 0; n < graph->vertex[i].degree; n++) {
    if (colors[graph->vertex[i].neighbor[n].dest] != 0) {
      is_available[colors[graph->vertex[i].neighbor[n].dest]] = true;
    }
  }
}
