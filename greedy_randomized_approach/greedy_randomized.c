#include "greedy_randomized.h"


bool is_in_constraint(int array_size, const cJSON *constrained_vertex_array, int current_vertex, int n, const int colors[n]){
    for (int i=0; i<array_size; i++){
        const cJSON* vertex = cJSON_GetArrayItem(constrained_vertex_array, i);
        if (colors[vertex->valueint] == colors[current_vertex] ) {
            return true;
        }
    }
    return false;
}

void greedy_randomized(int n, int graph_matrix[n][n], int colors[n], const cJSON* color_constraint){
    for (int i=0; i<n; i++){
        int r = (rand()%n)+1;

        colors[i] = r;
    }
    // traverse through graph edges
    for (int i=0; i<n; i++){
        for (int j=0; j<i; j++){

            char str_int[n+1];
            sprintf(str_int, "%d", i);
            const cJSON *constrained_vertex_array = cJSON_GetObjectItem(color_constraint, str_int);
            int array_size= cJSON_GetArraySize(constrained_vertex_array);

            bool is_constraint_wrong = is_in_constraint( array_size, constrained_vertex_array,  i,  n,  colors);

            if ((graph_matrix[i][j] ==1 && colors[i]!=colors[j]) || is_constraint_wrong){
                // check all neighbours + constraint and pick a color that is not used
                bool is_good = true;
                //colors
                for (int c =1; c<=n; c++){
                    // neighbors
                    is_good =true;
                    bool constrained_checked = false;
                    for (int w=0; w<n; w++){
                        // if color that we want to use is used by other vertex go to the next color
                        if (graph_matrix[i][w] ==1 && c == colors[w]) { is_good=false; break; }
                        else if (!constrained_checked){

                            bool is_constraint_true = false;
                            for (int m = 0; m< array_size; m++){
                                const cJSON* vertex = cJSON_GetArrayItem(constrained_vertex_array, m);
                                if (colors[vertex->valueint] == c ) {
                                    is_constraint_true = true;
                                    break;
                                }
                            }
                            if (is_constraint_true) {is_good= false; break;};
                            constrained_checked = true;
                        }

                    }
                    if (is_good) { colors[i] = c; break; };
                }

            }
        }
    }

}
