#include "serial_impl.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int compare(const void *a, const void *b) {
  int la = *(const int *)a;
  int lb = *(const int *)b;
  return (la > lb) - (la < lb);
}

void greedy(struct Graph *graph, int *colors) {
  bool *is_available = (bool *)malloc(graph->V * sizeof(bool));
  int i;

  for (i = 0; i < graph->V; i++) is_available[i] = true;

  // Assign the first color to the first vertex
  colors[0] = 1;

  // Assign colors to remaining V-1 vertices
  for (i = 1; i < graph->V; i++)
    first_available_color(graph, is_available, colors, i);
}

void ldf(struct Graph *graph, int *colors) {
  int colored_nodes = 0;
  init_weights(graph);
  int index;
  int k, i, max;
  bool *is_available = (bool *)malloc(graph->V * sizeof(bool));

  for (k = 0; k < graph->V; k++) is_available[k] = true;

  while (colored_nodes < graph->V) {
    max = -1;
    index = 0;

    for (i = 0; i < graph->V; i++) {
      if (colors[i] == 0) {
        if (graph->vertex[i].degree > max) {
          max = graph->vertex[i].degree;
          index = i;
        } else if (graph->vertex[i].degree == max &&
                   graph->vertex[i].weight > graph->vertex[index].weight) {
          index = i;
        }
      }
    }

    // color with lowest
    first_available_color(graph, is_available, colors, index);
    colored_nodes++;
  }
}

void jones_plassmann(struct Graph *graph, int *colors) {
  int i, j, k;
  int *j_colors,
      *neighbor_colors;  // j_colors holds the colors of process,
                         // neighbor_colors the colors of one of these
  int j_weight, num_colors, min_color = 0;
  bool j_weight_is_max;

  init_weights(graph);

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
      j_weight = graph->vertex[j].weight;
      j_weight_is_max = true;

      // initialize neighbor_colors
      neighbor_colors = (int *)malloc(graph->V * sizeof(int));
      memset(neighbor_colors, 0, graph->V * sizeof(int));
      num_colors = 0;
      // compare vertex weight to weights of its non-colored neighbors to see
      // if it is a maximum. Also gather the colors of all neighbors of the
      // vertex j that have been colored
      for (k = 0; k < graph->vertex[j].degree; k++) {
        // if neighbor is colored just add its color to the neighbor_colors
        int neighbor_id = graph->vertex[j].neighbor[k].dest;
        if (colors[neighbor_id] != 0) {
          neighbor_colors[num_colors++] = colors[neighbor_id];
        }
        // if the weights match, solve conflict by looking at the vertices
        // ids and taking the vertex with higher id as the max
        else if (j_weight < graph->vertex[neighbor_id].weight ||
                 (j_weight == graph->vertex[neighbor_id].weight &&
                  neighbor_id > j)) {
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
        graph->vertex[j].colored = true;
      }
      free(neighbor_colors);
    }
    // pass the temp_colors to colors vector
    for (k = 0; k < graph->V; k++) {
      colors[k] = j_colors[k];
    }
  }

  free(j_colors);
}
