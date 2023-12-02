#include "greedy_randomized.h"


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






void greedy_randomized(int n, int graph_matrix[n][n],int colors[n], const cJSON* color_constraint){
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
