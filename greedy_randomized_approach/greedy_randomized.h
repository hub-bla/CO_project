#include <stdio.h>
#include <stdlib.h>
#include "../json/cJSON.h"
#include <time.h>
#include <stdbool.h>

void greedy_randomized(int n, int graph_matrix[n][n], int colors[n], const cJSON* color_constraint);
