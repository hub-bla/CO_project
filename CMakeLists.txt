cmake_minimum_required(VERSION 3.26)
project(CO_graph_coloring C)

set(CMAKE_C_STANDARD 11)

add_executable(CO_graph_coloring main.c
        json/cJSON.c
        greedy_approach/greedy.c
        greedy_approach/greedy.h
        backtracking_approach/backtracking.c
        genetic_alg_approach/genetic.c)
