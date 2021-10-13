#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "serial_impl.h"

void help_message(char *exec) {
  char *title = "Serial Implementations of Graph Coloring\nUsage:\t";
  char *usage = " -a <algorithm> -f <input file>\n";
  char *opt1 =
      "Options:\t-a <algorithm>  : algorithm can be \"greedy\", \"jones\" or "
      "\"ldf\"\n";
  char *opt2 = "\t\t-f <input file> : set the path to the input file\n\n";
  printf("%s%s%s%s%s", title, exec, usage, opt1, opt2);
}

int main(int argc, char *argv[]) {
  int opt;
  char *algo = "", *filepath = "";
  double sw_start, sw_end, sw_exec;

  if (argc < 2) {
    help_message(argv[0]);
    return 0;
  }

  while ((opt = getopt(argc, argv, ":a:f:")) != -1) {
    switch (opt) {
      case 'a':
        algo = optarg;
        break;
      case 'f':
        filepath = optarg;
        break;
      case '?':
        printf("Invalid Option: %c\n\t", optopt);
        help_message(argv[0]);
        return 0;
    }
  }

  if (strcmp(filepath, "") == 0) {
    printf("No input file provided\n");
    help_message(argv[0]);
    return 0;
  }
  struct Graph *graph = graph_read(filepath);
  int *colors = (int *)malloc(graph->V * sizeof(int));
  memset(colors, 0, graph->V * sizeof(int));

  if (strcmp(algo, "greedy") == 0 || strcmp(algo, "jones") == 0 ||
      strcmp(algo, "ldf") == 0) {
    sw_start = get_timestamp();
    if (strcmp(algo, "greedy") == 0) greedy(graph, colors);
    if (strcmp(algo, "jones") == 0) jones_plassmann(graph, colors);
    if (strcmp(algo, "ldf") == 0) ldf(graph, colors);
    sw_end = get_timestamp();
    sw_exec = (sw_end - sw_start) / (1000);
    printf("Software execution time : %.6f ms elapsed\n", sw_exec);
    find_min_max(colors, graph->V);
    printerrors(graph, colors);
  } else {
    printf("Invalid algorithm: %s\n", algo);
    help_message(argv[0]);
  }
  return 0;
}
