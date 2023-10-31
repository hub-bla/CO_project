#!/bin/bash

python3 graph_generator.py  $1 $2


 
echo starting algs
./CO_graph_coloring graph.json  greedy_randomized
./CO_graph_coloring graph.json  greedy
./CO_graph_coloring graph.json  meta-heuristic
