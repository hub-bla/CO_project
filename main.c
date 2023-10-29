#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "json/cJSON.h"
#include "greedy_approach/greedy.h"
#include "backtracking_approach/backtracking.h"
cJSON* load_json_file(char* file_name);
int get_num_of_vertices(cJSON* json);
void get_graph_matrix(cJSON* json, int n,int graph_matrix[n][n]);
void print_graph_matrix(int n, int graph_matrix[n][n]);


int main() {
    cJSON* json = load_json_file("../graph.json");
    const cJSON* color_constraint = cJSON_GetObjectItem(json, "color_constraint");
    const int n = get_num_of_vertices(json);
    int graph_matrix[n][n];
    get_graph_matrix(json, n, graph_matrix);

    int colors[n];
    for (int i=0; i<n; i++){
        colors[i] = 0;
    }
    colors[0] = 1;
    backtracking_alg(n, graph_matrix, colors, color_constraint);

    for (int i=0; i<n; i++){
        printf("%d ", colors[i]);
    }
    return 0;
}



cJSON* load_json_file(char* file_name){
    FILE *fptr;
    fptr = fopen(file_name, "rb");
    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    char* json_file = (char*) malloc(file_size+1);
    fread(json_file, 1, file_size, fptr);

    cJSON *json = cJSON_Parse(json_file);
    printf("%s\n", cJSON_Print(json));
    fclose(fptr);
    free(json_file);
    return json;



}


int get_num_of_vertices(cJSON* json){
    const cJSON *n_json = NULL;
    n_json = cJSON_GetObjectItem(json, "n");
    const int n = n_json->valueint;
    return n;
}

void get_graph_matrix(cJSON* json,  int n,int graph_matrix[n][n]){
    const cJSON *graph_json = NULL;
    graph_json = cJSON_GetObjectItem(json, "graph");
    for (int i = 0; i<n; i++){
        const cJSON *row = NULL;
        row = cJSON_GetArrayItem(graph_json, i);
        for (int j = 0; j<n; j++){
            const cJSON *cell = NULL;
            cell = cJSON_GetArrayItem(row, j);
            graph_matrix[i][j] = cell->valueint;
        }
    }
}



void print_graph_matrix(int n, int graph_matrix[n][n]){
    for (int i = 0; i<n; i++) {
        for (int j=0; j<n; j++){
            printf("%d ", graph_matrix[i][j]);
        }
        printf("\n");
    }
}

