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
                if (vertex->valueint != -1 && colors[vertex->valueint] ==color ) {
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

bool traverse_graph(int vertex1,int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint){
    if (vertex1 == n){
        if (colors[n-1] == 0) return false;
        return true;
    }
    //look for colors
    for (int k=1; k<=n; k++){
        if(is_valid(k, vertex1, n, graph_matrix, colors, color_constraint)){
            colors[vertex1] = k;
            if(traverse_graph( vertex1+1, n,  graph_matrix, colors, color_constraint)) return true;
        }


    }

    return false;
}

void backtracking_alg(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint){
    traverse_graph(0,  n,  graph_matrix, colors,   color_constraint);
}