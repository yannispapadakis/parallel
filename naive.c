#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"

void greedy_coloring(struct Graph *graph, int *colors) {
  bool *is_available = (bool *)malloc(graph->V * sizeof(bool));

  unsigned int i;

  for (i = 0; i < graph->V; i++) is_available[i] = true;

  // Assign the first color to the first vertex
  colors[0] = 1;

  // Assign colors to remaining V-1 vertices
  for (i = 1; i < graph->V; i++) {

	first_available_color(graph, is_available, colors, i);

  }
  printf("Max Degree: %d\n", graph->maxDegree);
  find_min_max(colors, graph->V);
  printerrors(graph,colors);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <path/to/graph>\n", argv[0]);
    return 0;
  }

  struct Graph *graph = graph_read(argv[1]);

  printf("\nVertices = %d, Edges = %d\n", graph->V, graph->edges);

  int *colors = (int *)malloc(graph->V * sizeof(int));
  memset(colors, 0, graph->V * sizeof(int));

  greedy_coloring(graph, colors);

  return 0;
}
