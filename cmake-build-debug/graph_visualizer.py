import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
import json
import random
import sys

def show_graph_with_labels(adjacency_matrix, mylabels):
    rows, cols = np.where(adjacency_matrix == 1)
    edges = zip(rows.tolist(), cols.tolist())
    gr = nx.Graph()
    gr.add_edges_from(edges)
    nx.draw(gr, node_size=500, labels=mylabels, with_labels=True)
    plt.show()



def get_colors(vertices_colors):
    colors_d = dict()
    colors = []
    for color in vertices_colors:
        if color not in colors_d:
            colors_d[color] = (random.randint(0,255)/255, random.randint(0,255)/255, random.randint(0,255)/255)
        colors.append(colors_d[color])
    return colors
if __name__ == '__main__':
    if len(sys.argv) <3:
        sys.exit(1)

    f_graph = open(sys.argv[1], "r")

    graph_data = json.load(f_graph)
    graph_np= np.array(graph_data['graph'])

    f_colors = open(sys.argv[2], "r")
    coloring_data = json.load(f_colors)
    colors = coloring_data['colors']
    colors = get_colors(colors)
    g = nx.from_numpy_array(graph_np)
    plt.figure()
    plt.title(f"Przykładowy graf oraz liczba kolorów: {len(set(colors))}")
    nx.draw_networkx(g, with_labels=True, node_color=colors)
    A = nx.nx_agraph.to_agraph(g)

    A.write("k5.dot")
    plt.show()
    # print(g)
