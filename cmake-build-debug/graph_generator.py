import json
import numpy as np
import random
import sys

def create_neigh_with_specific_saturation(matrix_edge, sat, directed=False):
    neigh = [[0 for i in range(matrix_edge)]for j in range(matrix_edge)]
    n_of_edges = 0
    if (directed is True):
        n_of_edges = (matrix_edge*(matrix_edge-1))
    else:
        n_of_edges= (matrix_edge*(matrix_edge-1))/2

    ones = int(n_of_edges* sat)
    zeros =int( n_of_edges - ones)
    arr_of_ones = np.ones(shape=ones).astype('int32')
    arr_of_zeros = np.zeros(shape=zeros).astype('int32')
    zeros_and_ones =  np.concatenate((arr_of_ones, arr_of_zeros), axis=None)
    np.random.shuffle(zeros_and_ones)
    for i in range(0, matrix_edge):
        for j in range(0, i):
            if directed is True:
                direction = random.choice([-1,1])
                neigh[i][j] = int(zeros_and_ones[0]) * direction
                neigh[j][i] = neigh[i][j] *(-1)
            else:
                neigh[i][j] = int(zeros_and_ones[0])
                neigh[j][i] = int(zeros_and_ones[0])
            zeros_and_ones = zeros_and_ones[1:]
    return neigh



if __name__ == '__main__':
    if len(sys.argv) <3:
        sys.exit(1)

    n =int(sys.argv[1])
    saturation =  float(sys.argv[2])
    graph = create_neigh_with_specific_saturation(n, saturation)
    temp_dict = {i:[] for i in range(n)}
    for i in range(n):
        r =random.randint(0, n-1)
        prob = random.randint(1, 100)
        if prob<=20 and r!=i and r not in temp_dict[i]:
            temp_dict[i].append(r)
            temp_dict[r].append(i)

    color_constraint_dict = {key:sorted(val) for (key, val) in temp_dict.items()}
    to_json = {
        "n": n,
        "graph": graph,
        "color_constraint": color_constraint_dict
    }

    json_data = json.dumps(to_json)
    f = open("graph.json", "w")
    f.writelines(json_data)


