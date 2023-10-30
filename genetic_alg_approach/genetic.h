#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../json/cJSON.h"

void genetic_algorithm(int num_of_iterations, int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint);
int count_colors(int n, const int colors[n]);