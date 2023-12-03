#!/bin/bash

for i in {5..30..1}
do
echo generating graph
python3 graph_generator.py  $i $1

echo starting algs $i
if [ $i -lt 10 ]
then
echo backtracking
./CO_graph_coloring graph.json  backtracking
fi
echo greedy_randomized
./CO_graph_coloring graph.json  greedy_randomized
echo greedy
./CO_graph_coloring graph.json  greedy
echo meta-heuristic
./CO_graph_coloring graph.json  meta-heuristic

done