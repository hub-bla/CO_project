#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "json/cJSON.h"

void greedy_coloring(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint);
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
//    print_graph_matrix(n, graph_matrix);

    int colors[n];
    for (int i=0; i<n; i++){
        colors[i] = 0;
    }
    colors[0] = 1;
    greedy_coloring(n, graph_matrix, colors, color_constraint);

    for (int i=0; i<n; i++){
        printf("%d ", colors[i]);
    }
    return 0;
}

// tries to set colors one by one
// the number of colors might be higher than optimal value
// but it always will find some solution
void greedy_coloring(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint){
    for (int i=1; i<n; i++){
        for (int k=1; k<=n; k++){
            int is_good = 1;
            for (int j=0; j<i;j++) {


                if (graph_matrix[i][j] == 1 && colors[j] == k) {
                    is_good = 0;
                    break;
                }
                char str_int[n+1];
                sprintf(str_int, "%d", j);

                const cJSON *constrained_vertex_array = cJSON_GetObjectItem(color_constraint, str_int);
                int w = cJSON_GetArraySize(constrained_vertex_array);
                int is_constraint_true = 0;
                for (int m = 0; m< w; m++){
                    const cJSON* vertex = cJSON_GetArrayItem(constrained_vertex_array, m);
                    if (vertex->valueint != -1 && vertex->valueint==i && colors[j] ==k ) {
                        is_constraint_true = 1;
                        break;
                    }
                }
                if (is_constraint_true){
                    is_good = 0;
                    break;
                }
            }
            if (is_good) {
                colors[i] = k;
                break;
            }
        }
    }
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