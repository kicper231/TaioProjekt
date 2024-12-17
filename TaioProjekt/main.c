#include <stdbool.h>
#include <stdlib.h>

#include "algorithms.h"
#include "argparser.h"
#include "error.h"
#include "loader.h"
#include "generators.h"
#include "metric.h"
#include "size.h"
#include "timer.h"

void process_graphs(struct file* files, int files_number, bool size_flag, bool metric_flag, bool cycle_flag, bool hamilton_flag, bool hamilton_completion_flag, bool approx_flag);
void find_graph_maximal_cycle(graph_t* graph, bool approximation);
void free_resources(struct file* files, int files_number);
void find_and_count_hamiltonian_completion(graph_t* graph, bool approximation);
void find_graph_minimum_hamiltonian_completion(graph_t* graph, bool approximation);
void make_and_print_adjacency_matrix(graph_t* graph);

int main(int argc, char** argv)
{
    timer_init();
    if (argc < 2)
    {
        printf("No arguments passed!\n");
        return 0;
    }

    bool metric_flag = false, size_flag = false, approx_flag = false, cycle_flag = false, hamilton_flag = false, hamilton_completion_flag = false;
    int optind = 1;

    parse_flags(argc, argv, &metric_flag, &size_flag, &approx_flag, &cycle_flag, &hamilton_flag, &hamilton_completion_flag, &optind);

    int exit_code = 0;
    int files_number = metric_flag && argc - optind > 2 ? 2 : argc - optind;
    struct file* files = (struct file*)malloc((files_number) * sizeof(struct file));

    if (!files)
    {
        ERR("Memory allocation failed");
    }

    for (int i = 0; i < files_number; i++)
    {
        int read_result = file_read(&(files[i]), argv[i + optind]);

        if (0 != read_result)
        {
            files[i].graph_count = 0;
            files[i].graphs = NULL;
            continue;
        }
    }

    process_graphs(files, files_number, size_flag, metric_flag, cycle_flag, hamilton_flag, hamilton_completion_flag, approx_flag);

    free_resources(files, files_number);

    return exit_code;
}

void process_graphs(struct file* files, int files_number, bool size_flag, bool metric_flag, bool cycle_flag, bool hamilton_flag, bool hamilton_completion_flag, bool approx_flag)
{
    if (metric_flag)
    {
        if (files_number != 2)
        {
            ERR("Two first files and their first graphs have to be correctly allocated to calculate metric");
        }

        if (!files[0].graphs || !files[1].graphs || !files[0].graphs[0] || !files[1].graphs[0])
        {
            ERR("Two first files and their first graphs have to be correctly allocated to calculate metric");
        }

        int metric = 0, metric_approximation = 0;
        metric_calculate(files[0].graphs[0], files[1].graphs[0], &metric, &metric_approximation, approx_flag);
        if (approx_flag)
        {
            printf("The metric approximation of the first two graphs in the files is %d", metric_approximation);
        }
        else {
            printf("The metric of the first two graphs in the files is %d", metric);
        }

        return;
    }

    for (int i = 0; i < files_number; i++)
    {
        for (int j = 0; j < files[i].graph_count; j++)
        {
            printf("Processing graph %d from file %d \n", j + 1, i + 1);
            make_and_print_adjacency_matrix(files[i].graphs[j]);

            if (size_flag)
            {
                double graph_size = 0;
                size_calculate(files[i].graphs[j], &graph_size);
                printf("Size of graph is %f \n", graph_size);
            }
            else if (cycle_flag)
            {
                find_graph_maximal_cycle(files[i].graphs[j], approx_flag);
            }
            else if (hamilton_flag)
            {
                find_and_count_hamiltonian_completion(files[i].graphs[j], approx_flag);
            }
            else if (hamilton_completion_flag)
            {
                find_graph_minimum_hamiltonian_completion(files[i].graphs[j], approx_flag);
            }
        }
    }
}

void find_and_count_hamiltonian_completion(graph_t* graph, bool approximation)
{
    if (!graph)
    {
        return;
    }

    hamiltonian_data_t result;
    if (0 != hamiltonian_data_init(&result, graph))
    {
        return;
    }

    int n = graph->n;

    bool* visited = (bool*)calloc(n, sizeof(bool));
    if (!visited)
    {
        return;
    }

    stack_t stack;
    if (0 != stack_init(&stack, n))
    {
        free(visited);
        return;
    }

    adjacency_data_t adj_data;
    if (0 != adjacency_data_init(&adj_data, graph))
    {
        free(visited);
        stack_free(&stack);
        return;
    }

    TIMER(graph_find_and_count_hamiltonian_completion(&result, &adj_data, &stack, visited, approximation),
        "Minimum Hamiltonian completion");

    hamiltonian_completion_print(&result, &adj_data);

    hamiltonian_data_free(&result);
    stack_free(&stack);
    adjacency_data_free(&adj_data);
    free(visited);
}

void find_graph_maximal_cycle(graph_t* graph, bool approximation)
{

    if (!graph)
    {
        return;
    }

    stack_t stack;
    cycle_data_t result;

    if (0 != cycle_data_init(&result, graph))
    {
        return;
    }

    if (0 != stack_init(&stack, graph->n))
    {
        cycle_data_free(&result);
        return;
    };

    bool* visited = (bool*)calloc(graph->n, sizeof(bool));
    if (!visited)
    {
        ERR("Memory allocation for visited array failed");
    }

    TIMER(graph_find_maximal_cycle(graph, &result, &stack, visited, approximation),
        approximation ? "Approximate finding the longest cycles" : "Finding the longest cycles");

    cycle_data_print(&result);

    free(visited);
    stack_free(&stack);
    cycle_data_free(&result);
}

void free_resources(struct file* files, int files_number)
{
    if (!files)
        return;

    for (int i = 0; i < files_number; i++)
    {
        free_all_graphs(files[i].graphs, files[i].graph_count);
    }

    free(files);
}

void find_graph_minimum_hamiltonian_completion(graph_t* graph, bool approximation)
{

    int result;
    TIMER(graph_find_minimum_hamiltonian_completion(&result, graph, approximation), "Finding minimal hamiltonian completion");

    if (0 == result)
    {
        printf("The multigraph has a Hamiltonian cycle.\n");
    }
    else
    {
        printf("Minimum Hamiltonian completion is size of %d \n", result);
    }
}

void make_and_print_adjacency_matrix(graph_t* graph)
{
    int** matrix = (int**)malloc(sizeof(int*) * graph->n);
    if (!matrix)
    {
        ERR("Malloc failed!");
    }

    for (int i = 0; i < graph->n; i++)
    {
        matrix[i] = (int*)malloc(sizeof(int) * graph->n);

        if (!matrix[i])
        {
            ERR("Malloc failed!");
        }

        for (int j = 0; j < graph->n; j++)
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

        node_t* node = graph->edges[i];
        while (node)
        {
            matrix[i][node->vertex] = node->quantity;
            node = node->next;
        }
    }
    printf("Adjancency matrix: \n");
    for (int i = 0; i < graph->n; i++)
    {
        for (int j = 0; j < graph->n; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
        free(matrix[i]);
    }

    free(matrix);
}