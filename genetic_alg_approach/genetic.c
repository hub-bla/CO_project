#include "genetic.h"

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

void generate_individual(int n, int instance_arr[n]){
    for (int i=0; i<n; i++){
        int color = 1+ (rand()%n);
        instance_arr[i] =color;
    }
}

void insert_new_individual(int n, int population_size,  int fitness_arr[population_size], int population_arr[population_size][n], int ended_on){
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
// we want to minimize number of used colors and also find correct coloring
int evaluate_individual(int n,  const int instance_arr[n], int graph_matrix[n][n], const cJSON* color_constraint){
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


void mate_individual(int n, int new_individual[n],const int parent1[n], const int parent2[n]){
    for(int i=0; i<n; i++){
        int r = rand()%100;
        if (r<45) new_individual[i] = parent1[i];
        else if(r<90) new_individual[i] = parent2[i];
        else new_individual[i] = 1+ (rand()%n);
    }
}


void copy_elites(int n, int population_size,
                 int population_arr[population_size][n],
                 int new_generation[population_size][n],
                 const int fitness_arr[population_size],
                 int new_fitness_arr[population_size], int end){
    for(int i=0; i<end; i++){
        new_fitness_arr[i] = fitness_arr[i];
        for(int j=0; j<n; j++){
            new_generation[i][j] = population_arr[i][j];
        }
    }
};


void copy_generation(int n, int population_size,int new_generation[population_size][n],int population_arr[population_size][n], const int new_fitness_arr[n], int fitness_arr[n]){
    for (int i=0; i<population_size; i++){
        fitness_arr[i] = new_fitness_arr[i];
        for(int j=0; j<n; j++) population_arr[i][j] = new_generation[i][j];
    }
}



void genetic_algorithm(int num_of_iterations, int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint){
    int population_size = 100;
    int population_arr[population_size][n];
    int fitness_arr[population_size];
    //generate instance
    //evaluate it
    // place it in correct position in the array
    for (int index=0; index<population_size; index++) {
        generate_individual(n, population_arr[index]);
        fitness_arr[index]=evaluate_individual(n, population_arr[index], graph_matrix, color_constraint);
        insert_new_individual(n, population_size, fitness_arr, population_arr, index);
    };

    for (int iteration=0; iteration< num_of_iterations; iteration++){

        int new_generation[population_size][n];
        int new_fitness_arr[population_size];
        int s1 = ((10*population_size)/100);
        copy_elites(n, population_size, population_arr, new_generation, fitness_arr, new_fitness_arr,s1);

        for (int k=s1; k<population_size; k++){
            int r1 = rand()%(population_size/2);
            int r2 = rand()%(population_size/2);
            mate_individual(n,  new_generation[k], population_arr[r1], population_arr[r2]);
            new_fitness_arr[k] = evaluate_individual(n, new_generation[k],graph_matrix, color_constraint);
            insert_new_individual(n,population_size, new_fitness_arr, new_generation, k);
        }


        for (int k = 0; k< n; k++){
            colors[k] = population_arr[0][k];
        }
        if (iteration%100 ==0){
            printf("Iteration: %d, Fitness: %d\n", iteration, fitness_arr[0]);
        }
        copy_generation(n, population_size, new_generation, population_arr, new_fitness_arr, fitness_arr);

    }
}