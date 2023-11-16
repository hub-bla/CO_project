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
void greedy_randomizedd(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint);
bool check_if_coloring_is_valid(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint);
bool check_if_coloring(int n, int graph_matrix[n][n],int colors[n]);
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
    if (strcmp(argv[2], "greedy") == 0){
        colors[0] = 1;
        greedy_coloring(n, graph_matrix, colors, color_constraint);
    }else if (strcmp(argv[2], "backtracking") ==0){
        backtracking_alg(n, graph_matrix, colors, color_constraint);
    }else if (strcmp(argv[2], "greedy_randomized") ==0){
        greedy_randomized(n, graph_matrix, colors, color_constraint);
    }else if (strcmp(argv[2], "meta-heuristic") == 0){
        colors[0] = 1;
        greedy_coloring(n, graph_matrix, colors, color_constraint);
        genetic_algorithm(n*100, n, graph_matrix, colors, color_constraint);
    }else greedy_randomizedd(n, graph_matrix, colors, color_constraint);



    printf("Minimal colors using %s alg: %d\n", argv[2], count_colors(n, colors));
    printf("%d\n", check_if_coloring_is_valid(n, graph_matrix, colors, color_constraint));
//    printf("%d\n", check_if_coloring(n, graph_matrix, colors));
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


bool check_if_coloring(int n, int graph_matrix[n][n],int colors[n]){
    for (int i=0; i<n; i++){
        for (int j=0; j<i; j++){
            if (graph_matrix[i][j]==1 && colors[i]== colors[j]) return false;
        }
    }
    return true;
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

bool check_if_coloring_is_valid(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint){
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


bool all_visited(int n, const int visited[n]){
    for (int i=0; i<n; i++){
        if(visited[i] ==0 ) return false;
    }
    return true;
}




bool traverse_graph_greedy(int vertex1,int n, int *least_needed_colors, int graph_matrix[n][n],int visited[n], int colors[n], const cJSON* color_constraint){

    if (all_visited(n, visited)){
        return true;
    }
    int neighbors =0;
    int neighbors_arr[n];
    for (int i=0; i<n; i++){
        if(graph_matrix[vertex1][i]==1 && visited[i]!=1 ){
            neighbors_arr[neighbors] = i;
            neighbors+=1;
        }
    }
    while (neighbors > 0){
                int r = rand()%(neighbors);
                int j = neighbors_arr[r];
                //make sure we won't pick the same vertex again
                neighbors_arr[r] =neighbors_arr[neighbors-1];
                neighbors_arr[neighbors-1] =j;

                 //now we have  arr of not visited neighbors from 0 to neighbors
                char str_int[n+1];
                sprintf(str_int, "%d", j);
                const cJSON *constrained_vertex_array = cJSON_GetObjectItem(color_constraint, str_int);
                int array_size= cJSON_GetArraySize(constrained_vertex_array);

                for (int k=1; k<n; k++){


                    bool color_picked = true;
                    for (int m = 0; m<array_size; m++){
                        const cJSON* vertex = cJSON_GetArrayItem(constrained_vertex_array, m);
                        if (colors[vertex->valueint] == k ) {
                            color_picked =false;
                            break;
                        }
                    }
                    if (!color_picked) continue;
                    for (int w =0; w<n; w++){

                        if(graph_matrix[j][w] == 1 && k==colors[w]){
                            color_picked =false;
                            break;
                        }




                    }
                    if (color_picked) { colors[j] = k; break; };
                }

                visited[j]=1;
                neighbors-=1;
                if(traverse_graph_greedy(j, n, least_needed_colors, graph_matrix, visited, colors, color_constraint)) break;

    }



    return false;
}






void greedy_randomizedd(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint){
    int least_needed_colors =  n;
    int visited[n];
    for (int i=0; i<n; i++) visited[i]=0;

    for (int i=0; i<n; i++){
        if(visited[i] == 0){
            char str_int[n+1];
            sprintf(str_int, "%d", i);
            const cJSON *constrained_vertex_array = cJSON_GetObjectItem(color_constraint, str_int);
            int array_size= cJSON_GetArraySize(constrained_vertex_array);

            visited[i] = 1;
            for (int k=1; k<n; k++){
                bool color_picked = true;
                for (int j =0; j<n; j++){
                    if(graph_matrix[i][j] == 1 && k==colors[j]){
                            color_picked =false;
                         break;
                    }
                    for (int m = 0; m<array_size; m++){
                        const cJSON* vertex = cJSON_GetArrayItem(constrained_vertex_array, m);
                        if (colors[vertex->valueint] == k ) {
                            color_picked =false;
                            break;
                        }
                    }
                }
                if (color_picked) { colors[i] = k; break; };
            }
        }
        if (traverse_graph_greedy(i,  n,  &least_needed_colors,graph_matrix, visited, colors,  color_constraint)) break;

    }
}

