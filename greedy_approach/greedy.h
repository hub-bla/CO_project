#include "../json/cJSON.h"
#include <stdbool.h>
#include <stdio.h>
void greedy_coloring(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint);