
#include "metric.h"
#include "error.h"
#include "generators.h"
#include "math.h"
#include "model.h"


void metric_calculate_accurate(int** first_matrix, int** second_matrix, int* metric, int matrix_size, permutation_jta_data_t* permutation_jta_data)
{
    int permutations_number = 1;
    for (int i = 2; i <= matrix_size; i++)
    {
        permutations_number *= i;
    }

    int *permutation =  (int* )malloc(matrix_size * sizeof(int));
    for(int i = 0; i < matrix_size; i++)
    {
        permutation[i] = i;
    }
    metric_calculate_sum(first_matrix, second_matrix, metric, matrix_size, permutation);

    for (int i = 0; i < permutations_number - 1; i++)
    {
        permutation_jta_generator_generate(permutation_jta_data, permutation);

        int metric_partial_result;
        metric_calculate_sum(first_matrix, second_matrix, &metric_partial_result, matrix_size, permutation);

        if(metric_partial_result < *metric)
        {
            *metric = metric_partial_result;
        }        
    }
}

int metric_calculate(graph_t* first_graph, graph_t* second_graph, int* metric, int* metric_approximation, bool approx_flag)
{
    int first_size = first_graph->n;
    int second_size = second_graph->n;

    int matrix_size = first_size > second_size ? first_size : second_size;
    int** first_matrix = (int** )malloc(matrix_size * sizeof(int*));
    int** second_matrix = (int** )malloc(matrix_size * sizeof(int*));

    if (!first_matrix || !second_matrix)
    {
        ERR("Malloc failure for matrix rows!");
      
    }

    for (int i = 0; i < matrix_size; i++)
    {
        first_matrix[i] = (int*)malloc(matrix_size * sizeof(int));
        second_matrix[i] = (int*)malloc(matrix_size * sizeof(int));

        if (!first_matrix[i] || !second_matrix[i])
        {
            ERR("Malloc failure for matrix columns!");
        
        }
    }

    metric_make_adjacency_matrix(first_graph, matrix_size, first_matrix);
    metric_make_adjacency_matrix(second_graph, matrix_size, second_matrix);


    permutation_jta_data_t permutation_jta_data;
    permutation_jta_generator_init(&permutation_jta_data, matrix_size);
    if(approx_flag)
    {
        TIMER(metric_calculate_approximation(first_matrix, second_matrix, metric_approximation, matrix_size), "Approximate metric computation");
    }
    else 
    {
        TIMER(metric_calculate_accurate(first_matrix, second_matrix, metric, matrix_size, &permutation_jta_data), "Accurate metric computation");
    }
    
    for(int i = 0; i < matrix_size; i++)
    {
        free(first_matrix[i]);
        free(second_matrix[i]);
    }
    free(first_matrix);
    free(second_matrix);
    return 0;
}

void metric_make_adjacency_matrix(graph_t *graph, int size, int **matrix)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = 0; 
        }
    }

    for (int i = 0; i < graph->n; i++)
    {
        if (!graph->edges[i])
        {
            continue;
        }

        node_t *node = graph->edges[i];
        while (node)
        {
            matrix[i][node->vertex] = node->quantity;
            node = node->next;
        }
    }

}

void metric_calculate_approximation(int **first_graph_matrix, int **second_graph_matrix, int *metric_approximation, int matrix_size)
{
    int* first_graph_permutation = (int*) malloc(sizeof(int) * matrix_size);
    int* second_graph_permutation = (int*) malloc(sizeof(int) * matrix_size);
    metric_best_permutation(first_graph_matrix, matrix_size, first_graph_permutation);
    metric_best_permutation(second_graph_matrix, matrix_size, second_graph_permutation);

    int result = 0;
    for(int i = 0; i < matrix_size; i++)
    {
        for(int j = 0; j < matrix_size; j++)
        {
            result += abs(first_graph_matrix[first_graph_permutation[i]][first_graph_permutation[j]] 
                            - second_graph_matrix[second_graph_permutation[i]][second_graph_permutation[j]]);
        }
    }
    
    *metric_approximation = result;
}

void metric_calculate_sum(int **first_graph_matrix, int **second_graph_matrix, int *metric_approximation, int matrix_size, int *permutation)
{
    int result = 0;
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            result += abs(first_graph_matrix[permutation[i]][permutation[j]] - second_graph_matrix[i][j]);
        }
    }

    *metric_approximation = result;
}

void metric_best_permutation(int **matrix, int matrix_size, int *permutation)
{
    int *vertex_degrees = (int *)malloc(matrix_size * sizeof(int));

    for (int i = 0; i < matrix_size; i++)
    {
        vertex_degrees[i] = 0;
        for (int j = 0; j < matrix_size; j++)
        {
            vertex_degrees[i] += matrix[i][j];
        }
    }

    for (int i = 0; i < matrix_size; i++)
    {
        permutation[i] = i;
    }

    for (int i = 0; i < matrix_size - 1; i++)
    {
        for (int j = i + 1; j < matrix_size; j++)
        {
            if (vertex_degrees[permutation[i]] < vertex_degrees[permutation[j]])
            {
                int temp = permutation[i];
                permutation[i] = permutation[j];
                permutation[j] = temp;
            }
        }
    }

    free(vertex_degrees);
}