#include "backtracking.h"

bool is_valid(int color, int vertex1,int n, int graph_matrix[n][n], const int colors[n], const cJSON* color_constraint){
    bool constrained_checked = false;
    for (int i=0; i<n; i++){
        if(graph_matrix[vertex1][i] ==1 && colors[i]==color) return false;
        else if (!constrained_checked){
            char str_int[n+1];
            sprintf(str_int, "%d", vertex1);
            const cJSON *constrained_vertex_array = cJSON_GetObjectItem(color_constraint, str_int);
            int w = cJSON_GetArraySize(constrained_vertex_array);
            bool is_constraint_true = false;
            for (int m = 0; m< w; m++){
                const cJSON* vertex = cJSON_GetArrayItem(constrained_vertex_array, m);
                if (colors[vertex->valueint] ==color ) {
                    is_constraint_true = true;
                    break;
                }
            }
            if (is_constraint_true) return false;
            constrained_checked = true;
        }

    }
    return true;
}
int count_colors_backtracking(int n, const int colors[n]){
    int count_arr[n+1];
    int counter= 0;
    for (int i=0; i<n+1; i++) count_arr[i]=0;
    for (int i=0; i<n; i++){
        int color = colors[i];
        if(count_arr[color] == 0) {
            count_arr[color] =1;
            counter+=1;
        }
    }
    return counter;
}
bool traverse_graph(int vertex1,int n, int *least_needed_colors , int graph_matrix[n][n],int temp_colors[n], int colors[n], const cJSON* color_constraint){
    if (vertex1 == n){
        int color_counter = count_colors_backtracking(n, temp_colors);

        if (color_counter<*least_needed_colors) {
            for (int i = 0; i < n; i++) {
                colors[i] = temp_colors[i];
            };
            *least_needed_colors = color_counter;
        };
        return NULL;
    }
    //look for colors
    for (int k=1; k<=n; k++){
        if(is_valid(k, vertex1, n, graph_matrix, temp_colors, color_constraint)){
            temp_colors[vertex1] = k;
            traverse_graph( vertex1+1,  n,least_needed_colors,  graph_matrix, temp_colors, colors, color_constraint);
            temp_colors[vertex1] = 0;
        }


    }

    return NULL;
}



void backtracking_alg(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint){
    int least_needed_colors =  n;
    int temp_colors[n];
    for (int i=0; i<n; i++) temp_colors[i]=0;
    traverse_graph(0,  n,  &least_needed_colors,graph_matrix, temp_colors,colors,   color_constraint);
}