#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "graph.h"

int* init_weights(unsigned int V) {
	unsigned int i;
	int *weights = (int *)malloc(V * sizeof(int));
	for (i = 0; i < V; i++)
		weights[i] = rand() % (V * 1000);
	return weights;
}

void ldf(struct Graph *graph, int *colors) {
	unsigned int colored_nodes = 0;
	int *weights = init_weights(graph->V);
	int index;
	unsigned int k, i, max;
  bool *is_available = (bool *)malloc(graph->V * sizeof(bool));

  for (k = 0; k < graph->V; k++) is_available[k] = true;

	while (colored_nodes < graph->V) {
		max = 0;
		index = 0;

		// TODO change to qsort
		for (i = 0; i < graph->V; i++) {
			if (colors[i] == -1) {
				if (graph->array[i].neighbors > max) {
					max = graph->array[i].neighbors;
					index = i;
				}
				else if (graph->array[i].neighbors == max && weights[i] > weights[index]) {
					index = i;
				}
			}
		}

		//color with lowest
		first_available_color(graph, is_available, colors, index);
		colored_nodes++;

	}

  find_min_max(colors, graph->V);
  printerrors(graph, colors);

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

  ldf(graph, colors);

  return 0;
}
