#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"

void greedy_coloring(struct Graph *graph, int *colors) {
  bool *is_unavailable = (bool *)malloc(graph->V * sizeof(bool));

  unsigned int i, n;

  for (i = 0; i < graph->V; i++) is_unavailable[i] = false;

  // Assign the first color to the first vertex
  colors[0] = 0;

  // Assign colors to remaining V-1 vertices
  for (i = 1; i < graph->V; i++) {
    // set the colors of all adjacent vertices as unavailable
    for (n = 0; n < graph->array[i].neighbors; n++)
      if (colors[graph->array[i].head[n].dest] != -1)
        is_unavailable[colors[graph->array[i].head[n].dest]] = true;

    // find the first available color
    for (n = 1; n < graph->V; n++) {
      if (is_unavailable[n] == false) {
        break;
      }
    }
    colors[i] = n;

    // reset the color availability
    for (n = 0; n < graph->array[i].neighbors; n++)
      if (colors[graph->array[i].head[n].dest] != -1)
        is_unavailable[colors[graph->array[i].head[n].dest]] = false;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <path/to/graph>\n", argv[0]);
    return 0;
  }

  struct Graph *graph = graph_read(argv[1]);

  printf("\nVertices = %d, Edges = %d\n", graph->V, graph->edges);

  int *colors = (int *)malloc(graph->V * sizeof(int));
  memset(colors, -1, graph->V * sizeof(int));

  greedy_coloring(graph, colors);

  return 0;
}
