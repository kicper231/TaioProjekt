#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdbool.h>

#include "model.h"
#include "stack.h"

typedef struct cycle_data {
    int number_of_cycles;
    int vertex_count;
    int min_edge;
    int edges_count;
    int* cycle;
} cycle_data_t;

int cycle_data_init(cycle_data_t* cycle_data, graph_t* graph);
void cycle_data_free(cycle_data_t* cycle_data);
void cycle_data_print(const cycle_data_t* cycle_data);

typedef struct backtracking_data {
    graph_t* graph;
    int start;
    bool* visited;
    stack_t* stack;
    cycle_data_t* result;
} backtracking_data_t;

bool graph_find_maximal_cycle_including_start(backtracking_data_t* data, int vertex, int vertex_count, int min_edge, int edges_count, bool approximation);
bool graph_find_maximal_cycle(graph_t* graph, cycle_data_t* result, stack_t* stack, bool* visited, bool approximation);

typedef struct adjacency_data {
    int n;
    int* matrix;
} adjacency_data_t;

int adjacency_data_init(adjacency_data_t* adjacency_data, graph_t* graph);
void adjacency_data_free(adjacency_data_t* adjacency_data);
void adjacency_data_print(const adjacency_data_t* adjacency_data);

typedef struct tsp_data {
    int n;
    int set;
    int set_count;
    int target_set_count;
    int recursion_argument;
    const int* costs;
    int* results;
} tsp_data_t;

int tsp_data_init(tsp_data_t* tsp_data, adjacency_data_t* adjacency_data);
void tsp_data_free(tsp_data_t* tsp_data);

void tsp_recursion(tsp_data_t* tsp_data);
int graph_solve_tsp(int* result, adjacency_data_t* adjacency_data);
void mark_path_dfs(int v, int* vis, node_t** edges);
int heuristic_approximation(int* result, graph_t* graph);
int graph_find_minimum_hamiltonian_completion(int* result, graph_t* graph, bool approximation);

typedef struct hamiltonian_data {
    int completion;
    int number_of_completions;
    int* examplary_completion;
} hamiltonian_data_t;

int hamiltonian_data_init(hamiltonian_data_t* data, graph_t* graph);
void hamiltonian_data_free(hamiltonian_data_t* data);

typedef struct hamiltonian_recursion_data {
    hamiltonian_data_t* result;
    adjacency_data_t* adj_data;
    bool* visited;
    stack_t* stack;
    int start;
} hamiltonian_recursion_data_t;

bool hamiltonian_backtracking(hamiltonian_recursion_data_t* data, int vertex, int vertex_count, int edges_count, bool approximation);
int graph_find_and_count_hamiltonian_completion(hamiltonian_data_t *result, adjacency_data_t* adj_data, stack_t* stack, bool* visited, bool approximation);
void hamiltonian_completion_print(hamiltonian_data_t* data, adjacency_data_t* adj_data);

#endif
