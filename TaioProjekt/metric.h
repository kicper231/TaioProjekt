#ifndef METRIC_H
#define METRIC_H

#include "model.h"
#include "stdbool.h"
#include "generators.h"
#include "timer.h"

void metric_calculate_approximation(int** first_graph_matrix, int** second_graph_matrix, int* metric_approximation, int matrix_size);

void metric_calculate_sum(int **first_graph_matrix, int **second_graph_matrix, int *metric_approximation, int matrix_size, int *permutation);

void metric_make_adjacency_matrix(graph_t* graph, int size, int** matrix);

int metric_calculate(graph_t *first_graph, graph_t *second_graph, int *metric, int *metric_approximation, bool approx_flag);

void metric_best_permutation(int **matrix, int matrix_size, int *permutation);

#endif