#include "graph.h"

#include <math.h>
#include <stdint.h>
#include <string.h>

// A utility function that creates a graph of V vertices
struct Graph *createGraph(int V) {
  struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
  graph->V = V;
  graph->edges = 0;
  graph->maxDegree = 0;

  // Create an array of adjacency lists.  Size of array will be V
  graph->array = (struct AdjList *)malloc(V * sizeof(struct AdjList));
  int64_t *color_arr = (int64_t *)malloc(V * sizeof(int64_t));

  // Initialize each adjacency list as empty by making head as NULL
  for (int i = 0; i < V; ++i) {
    graph->array[i].head = NULL;
    graph->array[i].neighbors = 0;
    graph->array[i].VertexID = i;

    // Initialize color as unassigned
    color_arr[i] = -1;
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
  if (graph->array[src].head == NULL)
    graph->array[src].head =
        (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
  else
    graph->array[src].head = (struct AdjListNode *)realloc(
        graph->array[src].head,
        (graph->array[src].neighbors + 1) * sizeof(struct AdjListNode));

  graph->array[src].head[graph->array[src].neighbors] = *newNode;
  graph->array[src].neighbors++;

  // Store the maximimum vertex degree.
  if (graph->maxDegree < graph->array[src].neighbors)
    graph->maxDegree = graph->array[src].neighbors;

  // Since graph is undirected, add an edge from dest to src also
  newNode = newAdjListNode(src);

  if (graph->array[dest].head == NULL)
    graph->array[dest].head =
        (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
  else
    graph->array[dest].head = (struct AdjListNode *)realloc(
        graph->array[dest].head,
        (graph->array[dest].neighbors + 1) * sizeof(struct AdjListNode));

  graph->array[dest].head[graph->array[dest].neighbors] = *newNode;
  graph->array[dest].neighbors++;

  // Store the maximum vertex degree.
  if (graph->maxDegree < graph->array[dest].neighbors)
    graph->maxDegree = graph->array[dest].neighbors;

  graph->edges++;
}

void find_avg_degree(struct Graph *graph) {
  double sum = 0;
  unsigned int i, max = 0;
  int isolated = 0;

  // standard deviation
  double std = 0;
  double avg = (double)graph->edges / (double)graph->V;

  for (i = 0; i < graph->V; i++) {
    sum += (double)graph->array[i].neighbors;
    if (graph->array[i].neighbors > max) max = graph->array[i].neighbors;
    if (graph->array[i].neighbors == 0) isolated++;
    double help = (double)graph->array[i].neighbors;
    std = std + ((help - avg) * (help - avg));
  }

  std = std / (graph->V - 1);
  std = sqrt(std);

  graph->maxDegree = max;
  printf("Avg degree %.2lf sum %.2lf max degree %d\n", sum / graph->V, sum,
         max);
  printf("Avg degree %.2lf std(avg) %.2lf std(avg)/avg %.2lf\n", avg, std,
         std / avg);
  printf("Isolated nodes %d\n", isolated);
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
      /*			token = strtok(NULL, " ");
                              if (token != NULL)
                                      weight = atof(token);
      */
      edges++;
      addEdge(graph, source - 1, dest - 1);
    }
  }

  find_avg_degree(graph);
  return graph;
}
