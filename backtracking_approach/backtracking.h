#include "../json/cJSON.h"
#include <stdbool.h>
#include <stdio.h>


bool is_valid(int color, int vertex1,int n, int graph_matrix[n][n],const int colors[n], const cJSON* color_constraint);
bool traverse_graph(int vertex1,int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint);
void backtracking_alg(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint);