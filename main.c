#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "json/cJSON.h"
#include "greedy_approach/greedy.h"
#include "backtracking_approach/backtracking.h"
#include "genetic_alg_approach/genetic.h"
#include "greedy_randomized_approach/greedy_randomized.h"
#include <time.h>

#include <string.h>
cJSON* load_json_file(char* file_name);
int get_num_of_vertices(cJSON* json);
void get_graph_matrix(cJSON* json, int n,int graph_matrix[n][n]);
void print_graph_matrix(int n, int graph_matrix[n][n]);
bool check_if_coloring_is_valid(int n, int graph_matrix[n][n],const int colors[n], const cJSON* color_constraint);

int main(int argc, char* argv[]) {

    if (argc < 3) exit(1);
    srand(time(NULL));
    cJSON* json = load_json_file(argv[1]);
    const cJSON* color_constraint = cJSON_GetObjectItem(json, "color_constraint");
    const int n = get_num_of_vertices(json);
    int graph_matrix[n][n];
    get_graph_matrix(json, n, graph_matrix);

    int colors[n];
    for (int i=0; i<n; i++){
        colors[i] = 0;
    }
    clock_t t;
    t = clock();
    if (strcmp(argv[2], "greedy") == 0){
        colors[0] = 1;
        greedy_coloring(n, graph_matrix, colors, color_constraint);
    }else if (strcmp(argv[2], "backtracking") == 0){
        backtracking_alg(n, graph_matrix, colors, color_constraint);
    }else if (strcmp(argv[2], "greedy_randomized") == 0){
        greedy_randomized(n, graph_matrix, colors, color_constraint);
    }else if (strcmp(argv[2], "meta-heuristic") == 0){
        colors[0] = 1;
        greedy_coloring(n, graph_matrix, colors, color_constraint);
        genetic_algorithm(3000, n, graph_matrix, colors, color_constraint);
    }else return 0;

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("TIME\n %f\n", time_taken);
    printf("Minimal colors using %s alg: %d\n", argv[2], count_colors(n, colors));
    printf("%d\n", check_if_coloring_is_valid(n, graph_matrix, colors, color_constraint));
    cJSON *output = cJSON_CreateObject();
    cJSON* color_arr_json = cJSON_CreateIntArray(colors, n);
    cJSON* ref = cJSON_AddArrayToObject(output, "colors");
    color_arr_json->string = "colors";
    output->child = color_arr_json;
    int fd = creat("output.json", 0600);
    dup2(fd,1);
    printf("%s\n", cJSON_Print(output));

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

bool check_if_coloring_is_valid(int n, int graph_matrix[n][n],const int colors[n], const cJSON* color_constraint){
    int fitness_score = 0;
    for (int i=0; i<n; i++){

        for (int j=0; j<i; j++){
            if (graph_matrix[i][j]==1 && colors[i]==colors[j]) fitness_score+=1;
            char str_int[n+1];
            sprintf(str_int, "%d", i);
            const cJSON *constrained_vertex_array = cJSON_GetObjectItem(color_constraint, str_int);
            int w = cJSON_GetArraySize(constrained_vertex_array);
            for (int m = 0; m< w; m++){
                const cJSON* vertex = cJSON_GetArrayItem(constrained_vertex_array, m);
                if (vertex->valueint == j && colors[j] == colors[i]) {
                    fitness_score+=1;
                    break;
                }
            }
        }
    }
    if (fitness_score ==0) {
        return true;
    }
    return false;
}



