#include <stdbool.h>
#include <string.h>

#include "graph.h"

int compare(const void *a, const void *b) {
  int la = *(const int *)a;
  int lb = *(const int *)b;
  return (la > lb) - (la < lb);
}

void jones_plassmann(struct Graph *graph, int *weights, int *colors) {
  unsigned int i, j, k;
  int *j_colors,
      *neighbor_colors;  // j_colors holds the colors of process,
                         // neighbor_colors the colors of one of these
  int j_weight, num_colors, min_color = 0;
  bool j_weight_is_max;

  // Go through the vertices of this process and compare their weights with
  // neighboring vertices to find which of them are local maxima. Those form
  // the independent set in each iteration
  // The minimum number of colors, i.e. the chromaticity of the graph
  // can not be larger than chromaticity_upper so only iterate that many times

  j_colors = (int *)malloc(graph->V * sizeof(int));  // temp vector for colors
  memset(j_colors, 0, graph->V * sizeof(int));

  // minimum number of colors required to color the graph --> maxDegree
  for (i = 0; i < graph->maxDegree; i++) {
    // for each vertex of this process
    for (j = 0; j < graph->V; j++) {
      // get the vertex weight
      j_weight = weights[j];
      j_weight_is_max = true;

      // initialize neighbor_colors
      neighbor_colors =
          (int *)malloc(graph->V * sizeof(int));  // graph->array[j].neighbors
      memset(neighbor_colors, 0, graph->V * sizeof(int));
      num_colors = 0;
      // compare vertex weight to weights of its non-colored neighbors to see
      // if it is a maximum. Also gather the colors of all neighbors of the
      // vertex j that have been colored
      for (k = 0; k < graph->array[j].neighbors; k++) {
        // if neighbor is colored just add its color to the neighbor_colors
        if (colors[graph->array[j].head[k].dest] != 0) {
          neighbor_colors[num_colors++] = colors[graph->array[j].head[k].dest];
        }
        // if the weights match, solve conflict by looking at the vertices
        // ids and taking the vertex with higher id as the max
        else if (j_weight < weights[graph->array[j].head[k].dest] ||
                 (j_weight == weights[graph->array[j].head[k].dest] &&
                  graph->array[j].head[k].dest > j)) {
          j_weight_is_max = false;
          break;
        }
      }
      // if the vertex weight is a max and vertex hasnt been colored,
      // color it with the smallest color possible that is not one of
      // neighbor_colors
      if (j_weight_is_max == true && colors[j] == 0) {
        // find smallest color to assign to the j vertex
        // that color is either
        // a)1 if none of the neighbors is colored or the smallest color of a
        // neighbor is >1 b)In between a color in the array of neighbors colors
        // if there is a gap between two of the (sorted) neighbors colors c) 1
        // more than the last color in the sorted array of neighbors colors

        // sort neighbors colors.
        qsort(neighbor_colors, num_colors, sizeof(int), compare);
        if (num_colors == 0 || neighbor_colors[0] > 1)
          min_color = 1;
        else {
          for (k = 0; k < graph->V; k++) {
            if (k < graph->V - 1 &&
                (neighbor_colors[k + 1] - neighbor_colors[k] > 1)) {
              min_color = neighbor_colors[k] + 1;
              break;
            } else {
              min_color = neighbor_colors[num_colors - 1] + 1;
            }
          }
        }
        j_colors[j] = min_color;
      }
      free(neighbor_colors);
    }
    // pass the temp_colors to colors vector
    for (k = 0; k < graph->V; k++) {
      colors[k] = j_colors[k];
    }
  }

  // check if a vertice has same color with a neighbor
  for (i = 0; i < graph->V; i++) {
    for (j = 0; j < graph->array[i].neighbors; j++) {
      if (colors[graph->array[i].VertexID] ==
          colors[graph->array[i].head[j].dest])
        printf("Error between %d and %d\n", graph->array[i].VertexID,
               graph->array[i].head[j].dest);
    }
  }
  free(j_colors);
}

int *weights, *colors;  // random weights assigned at the beginning + colors
                        // assigned to each vertex

int main(int argc, char *argv[]) {
  if (argc < 2) printf("Usage: %s <path/to/graph>\n", argv[0]);

  struct Graph *graph = graph_read(argv[1]);

  printf("\nVertices=%d Edges=%d\n", graph->V, graph->edges);

  unsigned int i;

  weights = (int *)malloc(graph->V * sizeof(int));
  // Jones-Plassman needs to generate random weights which are a permutation of
  // the vertices
  for (i = 0; i < graph->V; i++) {
    weights[i] = rand() % (graph->V * 1000);
  }

  colors = (int *)malloc(graph->V * sizeof(int));
  memset(colors, 0, graph->V * sizeof(int));

  jones_plassmann(graph, weights, colors);

  return 0;
}
