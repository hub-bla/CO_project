#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "json/cJSON.h"
#include "greedy_approach/greedy.h"
#include "backtracking_approach/backtracking.h"
#include <time.h>
#include <stdlib.h>
cJSON* load_json_file(char* file_name);
int get_num_of_vertices(cJSON* json);
void get_graph_matrix(cJSON* json, int n,int graph_matrix[n][n]);
void print_graph_matrix(int n, int graph_matrix[n][n]);
void genetic_algorithm(int num_of_iterations, int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint);
bool check_if_coloring_is_valid(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint);
int count_colors(int n, const int colors[n]);
int main() {
    srand(time(NULL));
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
//    greedy_coloring(n, graph_matrix, colors, color_constraint);
    backtracking_alg(n, graph_matrix, colors, color_constraint);
//    genetic_algorithm(1000, n, graph_matrix, colors, color_constraint);

    printf("Genetic colors: %d\n", count_colors(n, colors));
    printf("%d\n", check_if_coloring_is_valid(n, graph_matrix, colors, color_constraint));
    for (int i=0; i<n; i++){
        printf("%d ", colors[i]);
    }

    return 0;
}

int count_colors(int n, const int colors[n]){
    int temp_arr[n+1];
    int counter = 0;
    for (int i=0; i<(n+1); i++) temp_arr[i] = 0;
    for (int j =0; j<n; j++){
        int color = colors[j];
        if (temp_arr[color] ==0) { temp_arr[color] = 1; counter+=1; };
    }
    return counter;
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


void generate_instance(int n, int instance_arr[n]){
    for (int i=0; i<n; i++){
        int color = 1+ (rand()%n);
        instance_arr[i] =color;
    }
}

void insert_new_instance(int n, int population,  int fitness_arr[population], int population_arr[population][n], int ended_on){
    for (int i=ended_on; i>0; i--){
        if (fitness_arr[i]<fitness_arr[i-1]) {
            int temp = fitness_arr[i];
            fitness_arr[i] = fitness_arr[i-1];
            fitness_arr[i-1] = temp;

            for (int j =0; j<n; j++){
                int temp2 = population_arr[i][j];
                population_arr[i][j] = population_arr[i-1][j];
                population_arr[i-1][j] = temp2;
            }

        }else if(fitness_arr[i] >=fitness_arr[i-1]){
            break;
        }
    }
}

int evaluate_instance(int n,  const int instance_arr[n], int graph_matrix[n][n], const cJSON* color_constraint){
    int fitness_score = 0;
    int num_colors = count_colors(n, instance_arr);
    for (int i=0; i<n; i++){
        for (int j=0; j<i; j++){
            if (graph_matrix[i][j]==1 && instance_arr[i]==instance_arr[j]) fitness_score+=1;
            char str_int[n+1];
            sprintf(str_int, "%d", i);
            const cJSON *constrained_vertex_array = cJSON_GetObjectItem(color_constraint, str_int);
            int w = cJSON_GetArraySize(constrained_vertex_array);
            for (int m = 0; m< w; m++){
                const cJSON* vertex = cJSON_GetArrayItem(constrained_vertex_array, m);
                if (vertex->valueint == j && instance_arr[j] == instance_arr[i]) {
                    fitness_score+=1;
                    break;
                }
            }
        }
    }


    return (fitness_score+num_colors);
}

//int fitness

void mate_individuals(int n, int new_individual[n],const int parent1[n], const int parent2[n]){
    for(int i=0; i<n; i++){
        int r = rand()%100;
        if (r<45) new_individual[i] = parent1[i];
        else if(r<90) new_individual[i] = parent2[i];
        else new_individual[i] = 1+ (rand()%n);
    }
}


void copy_elites(int n, int population, int population_arr[population][n], int new_generation[population][n],const int fitness_arr[population],int new_fitness_arr[population],int end){
    for(int i=0; i<end; i++){
        new_fitness_arr[i] = fitness_arr[i];
        for(int j=0; j<n; j++){
            new_generation[i][j] = population_arr[i][j];
        }
    }
};


void copy_generation(int n, int population,int new_generation[population][n],int population_arr[population][n], const int new_fitness_arr[n], int fitness_arr[n]){
    for (int i=0; i<population; i++){
        fitness_arr[i] = new_fitness_arr[i];
        for(int j=0; j<n; j++) population_arr[i][j] = new_generation[i][j];
    }
}



void genetic_algorithm(int num_of_iterations, int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint){
    int population = 100;
    int population_arr[population][n];
    int fitness_arr[population];
    //generate instance
    //evaluate it
    // place it in correct position in the array
    for (int index=0; index<population; index++) {
        generate_instance(n, population_arr[index]);
        fitness_arr[index]=evaluate_instance(n, population_arr[index], graph_matrix, color_constraint);
        insert_new_instance(n, population, fitness_arr, population_arr, index);
    };

    for (int i=0; i< num_of_iterations; i++){



        int new_generation[population][n];
        int new_fitness_arr[population];
        int s1 = ((10*population)/100);
        copy_elites(n, population, population_arr, new_generation, fitness_arr, new_fitness_arr,s1);

        for (int k=s1; k<population; k++){
            int r1 = rand()%50;
            int r2 = rand()%50;
            mate_individuals(n,  new_generation[k], population_arr[r1], population_arr[r2]);
            new_fitness_arr[k] = evaluate_instance(n, new_generation[k],graph_matrix, color_constraint);
            insert_new_instance(n,population, new_fitness_arr, new_generation, k);
        }


        for (int k = 0; k< n; k++){
            colors[k] = population_arr[0][k];
        }
        if (i%100 ==0){
            printf("Iteration: %d, Fitness: %d\n", i, fitness_arr[0]);
        }
        copy_generation(n, population, new_generation, population_arr, new_fitness_arr, fitness_arr);

    }
}

