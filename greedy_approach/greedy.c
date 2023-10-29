#include "greedy.h"
// tries to set colors one by one
// the number of colors might be higher than optimal value
// but it always will find some solution
void greedy_coloring(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint){
    for (int i=1; i<n; i++){
        for (int k=1; k<=n; k++){
            bool is_good = true;
            for (int j=0; j<i;j++) {


                if (graph_matrix[i][j] == 1 && colors[j] == k) {
                    is_good = false;
                    break;
                }
                char str_int[n+1];
                sprintf(str_int, "%d", j);
                const cJSON *constrained_vertex_array = cJSON_GetObjectItem(color_constraint, str_int);
                int w = cJSON_GetArraySize(constrained_vertex_array);
                bool is_constraint_true = false;
                for (int m = 0; m< w; m++){
                    const cJSON* vertex = cJSON_GetArrayItem(constrained_vertex_array, m);
                    if (vertex->valueint != -1 && vertex->valueint==i && colors[j] ==k ) {
                        is_constraint_true = true;
                        break;
                    }
                }
                if (is_constraint_true){
                    is_good = false;
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
