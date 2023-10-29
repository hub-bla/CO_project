#include "greedy.h"
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
