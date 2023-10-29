cmake_minimum_required(VERSION 3.26)
project(CO_graph_coloring C)

set(CMAKE_C_STANDARD 11)

add_executable(CO_graph_coloring main.c
        json/cJSON.c
        greedy_aproach/greedy.c
        greedy_aproach/greedy.h)
