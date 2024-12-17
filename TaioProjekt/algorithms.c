#include "algorithms.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "model.h"
#include "stack.h"
#include "timer.h"

int cycle_data_init(cycle_data_t* cycle_data, graph_t* graph)
{
    if(!cycle_data || !graph)
    {
        return 1;
    }

    cycle_data->vertex_count = 0;
    cycle_data->min_edge = INT_MAX;
    cycle_data->edges_count = 0;
    cycle_data->number_of_cycles = 0;
    cycle_data->cycle = (int*)malloc(graph->n * sizeof(int));
    if(!cycle_data->cycle)
    {
        ERR("Malloc failure!");
    }
    return 0;
}

void cycle_data_free(cycle_data_t* cycle_data)
{
    if(cycle_data && cycle_data->cycle)
    {
        free(cycle_data->cycle);
        cycle_data->cycle = NULL;
    }
}

void cycle_data_print(const cycle_data_t *cycle_data)
{
    if (!cycle_data)
    {
        printf("Cycle data is NULL\n");
        return;
    }

    if (cycle_data->number_of_cycles == 0)
    {
        printf("Cycle not found in the graph\n");
        return;
    }

    printf("Length: (%d, %d, %d)\n", cycle_data->vertex_count, cycle_data->min_edge, cycle_data->edges_count);
    printf("Number of cycles: %d\n", cycle_data->number_of_cycles);

    if (cycle_data->cycle)
    {
        for (int i = 0; i < cycle_data->vertex_count; i++)
        {
            printf("%d ", cycle_data->cycle[i]);
        }
        printf("\n");
    }
}

bool graph_find_maximal_cycle_including_start(backtracking_data_t* data, int vertex, int vertex_count, int min_edge, int edges_count, bool approximation)
{
    if(!data->graph || !data->visited || !data->stack || !data->result || !data->result->cycle)
    {
        return false;
    }
    
    data->visited[vertex] = true;
    stack_push(data->stack, vertex);
    vertex_count++;

    bool found = false;
    node_t* edge_to_start = NULL;
    node_t* neighbor = data->graph->edges[vertex];
    while(neighbor)
    {
        if(neighbor->vertex == data->start)
        {
            edge_to_start = neighbor;
        }
        else if(!data->visited[neighbor->vertex])
        {
            if(graph_find_maximal_cycle_including_start(data, neighbor->vertex, vertex_count, 
                                                        min_edge <= neighbor->quantity ? min_edge : neighbor->quantity, 
                                                        edges_count + neighbor->quantity, approximation))
            {
                found = true;
            }
        }
        neighbor = neighbor->next;
    }

    if(!found && edge_to_start)
    {   
        min_edge = min_edge <= edge_to_start->quantity ? min_edge : edge_to_start->quantity;
        edges_count += edge_to_start->quantity;

        if(data->result->vertex_count < vertex_count ||
          (data->result->vertex_count == vertex_count && 
          (data->result->min_edge < min_edge || 
          (data->result->min_edge == min_edge && 
           data->result->edges_count < edges_count))))
        {
            data->result->vertex_count = vertex_count;
            data->result->min_edge = min_edge;
            data->result->edges_count = edges_count;
            data->result->number_of_cycles = 1;
            memcpy(data->result->cycle, data->stack->array, sizeof(int) * vertex_count);
            found = true;
        }
        else if(data->result->vertex_count == vertex_count && 
                data->result->min_edge == min_edge &&
                data->result->edges_count == edges_count)
        {
            data->result->number_of_cycles++;
            found = true;
        }
    }

    stack_pop(data->stack);
    if(!approximation)
    {
        data->visited[vertex] = false;
    }

    return found;
}

bool graph_find_maximal_cycle(graph_t* graph, cycle_data_t* result, stack_t* stack, bool* visited, bool approximation)
{
    if(!graph || !graph->edges || 
        !stack || !result || !result->cycle || !visited)
    {
        return false;
    }

    int n = graph->n;

    backtracking_data_t data;
    data.graph = graph;
    data.visited = visited;
    data.stack = stack;
    data.result = result;

    for(int v = 0; v < n; v++)
    {
        data.start = v;
        graph_find_maximal_cycle_including_start(&data, v, 0, INT_MAX, 0, approximation);
        if(!approximation)
        {
            data.visited[v] = true;
        }
        else
        {
            memset(data.visited, 0, n * sizeof(bool));
        }
    }

    return true;
}


int adjacency_data_init(adjacency_data_t* adjacency_data, graph_t* graph)
{
    if (!adjacency_data || !graph)
    {
        return 1;
    }

    adjacency_data->n = graph->n;
    adjacency_data->matrix = (int*)calloc(graph->n * graph->n, sizeof(int));
    if (!adjacency_data->matrix)
    {
        ERR("Malloc failure!");
    }

    for (int v = 0; v < graph->n; v++)
    {
        node_t* node = graph->edges[v];
        while (node)
        {
            adjacency_data->matrix[v * graph->n + node->vertex] = node->quantity;
            node = node->next;
        }
    }

    return 0;
}

void adjacency_data_free(adjacency_data_t* adjacency_data)
{
    if (adjacency_data && adjacency_data->matrix)
    {
        free(adjacency_data->matrix);
        adjacency_data->matrix = NULL;
    }
}

void adjacency_data_print(const adjacency_data_t* adjacency_data)
{
    if (!adjacency_data)
    {
        printf("Adjacency data is NULL\n");
        return;
    }

    printf("n: %d\n", adjacency_data->n);

    if (adjacency_data->matrix)
    {
        for (int i = 0; i < adjacency_data->n; i++)
        {
            for (int j = 0; j < adjacency_data->n; j++)
            {
                printf("%d ", adjacency_data->matrix[i * adjacency_data->n + j]);
            }
            printf("\n");
        }
    }

    printf("\n");
}


int tsp_data_init(tsp_data_t* tsp_data, adjacency_data_t* adjacency_data)
{
    if (!tsp_data || !adjacency_data)
    {
        return 1;
    }

    if (adjacency_data->n > 25)
    {
        ERR("TSP data is only available for graphs with no more than 25 vertices!");
    }

    tsp_data->n = adjacency_data->n;
    tsp_data->costs = adjacency_data->matrix;
    tsp_data->set = 0;
    tsp_data->set_count = 0;
    tsp_data->target_set_count = 0;
    tsp_data->recursion_argument = 0;

    tsp_data->results = (int*)malloc((((1 << tsp_data->n) - 1) * tsp_data->n + tsp_data->n) * sizeof(int));
    if (!tsp_data->results)
    {
        ERR("Malloc failure!");
    }

    return 0;
}

void tsp_data_free(tsp_data_t* tsp_data)
{
    if(!tsp_data)
    {
        return;
    }
    if(tsp_data->results)
    {
        free(tsp_data->results);
        tsp_data->results = NULL;
    }
}


void tsp_recursion(tsp_data_t* tsp_data)
{
    if (tsp_data->set_count == tsp_data->target_set_count)
    {
        for (int i = 1; i < tsp_data->n; i++)
        {
            int subset = tsp_data->set & ~(1 << i);
            if (subset == tsp_data->set) continue;

            int min_value = INT_MAX;
            for (int j = 1; j < tsp_data->n; j++)
            {
                if (!((tsp_data->set >> j) & 1)) continue;
                if (j == i) continue;

                int value = tsp_data->results[subset * tsp_data->n + j] + tsp_data->costs[j * tsp_data->n + i];

                if (value < min_value)
                {
                    min_value = value;
                }
            }
            tsp_data->results[tsp_data->set * tsp_data->n + i] = min_value;
        }
        
        return;
    }

    if (tsp_data->recursion_argument == tsp_data->n)
    {
        return;
    }

    tsp_data->recursion_argument++;

    tsp_data->set |= 1 << (tsp_data->recursion_argument - 1);
    tsp_data->set_count++;
    tsp_recursion(tsp_data);
    tsp_data->set &= ~(1 << (tsp_data->recursion_argument - 1));
    tsp_data->set_count--;

    if (tsp_data->set_count + tsp_data->n - tsp_data->recursion_argument >= tsp_data->target_set_count)
    {
        tsp_recursion(tsp_data);
    }

    tsp_data->recursion_argument--;
}

int graph_solve_tsp(int* result, adjacency_data_t* adjacency_data)
{
    if (!result || !adjacency_data)
    {
        return 1;
    }

    if (adjacency_data->n == 1)
    {
        *result = 0;
        return 0;
    }

    tsp_data_t tsp_data;
    if(0 != tsp_data_init(&tsp_data, adjacency_data))
    {
        return -1;
    }

    tsp_data.results[(1 << 0) * tsp_data.n + 0] = 0;
    for (int i = 1; i < tsp_data.n; i++)
    {
        tsp_data.results[(1 << i) * tsp_data.n + i] = adjacency_data->matrix[0 * tsp_data.n + i];
    }

    for (int i = 2; i < tsp_data.n; i++)
    {
        tsp_data.target_set_count = i;
        tsp_data.recursion_argument = 1;
        tsp_recursion(&tsp_data);
    }
    
    int min_value = INT_MAX;
    int set = (1 << tsp_data.n)-1 - 1;
    for (int i = 1; i < tsp_data.n; i++)
    {
        int value = tsp_data.results[set * tsp_data.n + i] + tsp_data.costs[i * tsp_data.n + 0];
        if (value < min_value)
        {
            min_value = value;
        }
    }
    *result = min_value;
    
    tsp_data_free(&tsp_data);
    return 0;
}

void mark_path_dfs(int v, int* vis, node_t** edges)
{
    if (vis[v])
    {
        return;
    }

    vis[v] = 1;
    
    node_t* neigh = edges[v];
    while (neigh)
    {
        mark_path_dfs(neigh->vertex, vis, edges);
        neigh = neigh->next;
    }
}

int heuristic_approximation(int* result, graph_t* graph)
{
    *result = 1;
    int* vis = (int*)malloc(graph->n * sizeof(int));
    if (!vis)
    {
        ERR("Malloc failure!"); // TODO: return error
    }

    for (int i = 0; i < graph->n; i++)
    {
        if(!vis[i])
        {
            continue;
        }
        else
        {
            (*result)++;
            mark_path_dfs(i, vis, graph->edges);
        }
    }

    free(vis);
    
    return 0;
}

int graph_find_minimum_hamiltonian_completion(int* result, graph_t* graph, bool approximation)
{
    if (!graph || !result)
    {
        return 1;
    }

    if(approximation)
    {
        int code;
        TIMER(code = heuristic_approximation(result, graph), "Heuristic hamiltonian completion size");
        return heuristic_approximation(result, graph);
    }

    adjacency_data_t adjacency_data_complement_graph;
    if(0 != adjacency_data_init(&adjacency_data_complement_graph, graph))
    {
        return -1;
    }

    for (int i = 0; i < adjacency_data_complement_graph.n; i++)
    {
        for (int j = 0; j < adjacency_data_complement_graph.n; j++)
        {
            int* value = &(adjacency_data_complement_graph.matrix[i * adjacency_data_complement_graph.n + j]);
            *value = *value > 0 ? 0 : 1;
        }
    }

    TIMER(graph_solve_tsp(result, &adjacency_data_complement_graph), "Minimum Hamiltonian completion");
    adjacency_data_free(&adjacency_data_complement_graph);
    return 0;
}

int hamiltonian_data_init(hamiltonian_data_t *data, graph_t* graph)
{
    if(!data || !graph)
    {
        return 1;
    }

    data->completion = INT_MAX;
    data->number_of_completions = 0;
    data->examplary_completion = (int*)malloc(graph->n * sizeof(int));

    return NULL == data->examplary_completion ? -1 : 0;
}

void hamiltonian_data_free(hamiltonian_data_t *data)
{
    if(data)
    {
        if(data->examplary_completion)
        {
            free(data->examplary_completion);
            data->examplary_completion = NULL;
        }
    }
}

bool hamiltonian_backtracking(hamiltonian_recursion_data_t* data, int vertex, int vertex_count, int edges_count, bool approximation)
{
    if(!data || !data->result || !data->stack || !data->adj_data || !data->visited)
    {
        return false;
    }

    int n = data->adj_data->n;

    stack_push(data->stack, vertex);
    data->visited[vertex] = true;

    if(vertex_count == n - 1)
    {
        edges_count += data->adj_data->matrix[vertex * n + data->start];
        int current_completion = n - edges_count;
        if(current_completion < data->result->completion)
        {
            data->result->completion = current_completion;
            data->result->number_of_completions = 1;
            memcpy(data->result->examplary_completion, data->stack->array, n * sizeof(int));
        }
        else if(current_completion == data->result->completion)
        {
            data->result->number_of_completions++;
        }
    }
    else
    {
        if(!approximation)
        {
            for(int i = 0; i < n; i++)
            {
                if(!data->visited[i])
                {
                    hamiltonian_backtracking(data, i, vertex_count + 1, edges_count + data->adj_data->matrix[vertex * n + i], approximation);
                }
            }
        }
        else
        {
            for(int weight = 1; weight >= 0; weight--)
            {
                for(int i = 0; i < n; i++)
                {
                    if(!data->visited[i] && weight == data->adj_data->matrix[vertex * n + i])
                    {
                        hamiltonian_backtracking(data, i, vertex_count + 1, edges_count + weight, approximation);
                    }
                }
            }
        }
    }

    stack_pop(data->stack);
    if(!approximation)
    {
        data->visited[vertex] = false;
    }

    return true;
}

int graph_find_and_count_hamiltonian_completion(hamiltonian_data_t *result, adjacency_data_t* adj_data, stack_t* stack, bool* visited, bool approximation)
{
    if(!result || !adj_data || !stack || !visited)
    {
        return 1;
    }

    int n = adj_data->n;

    hamiltonian_recursion_data_t data;
    data.visited = visited;
    data.stack = stack;
    data.adj_data = adj_data;
    data.result = result;

    for(int i = 0; i < n * n; i++)
    {
        adj_data->matrix[i] = adj_data->matrix[i] != 0 ? 1 : 0;
    }

    if(approximation)
    {
        for(int v = 0; v < n; v++)
        {
            data.start = v;
            hamiltonian_backtracking(&data, v, 0, 0, approximation);
            memset(data.visited, 0, n * sizeof(bool));
        }
    }
    else
    {
        data.start = 0;
        hamiltonian_backtracking(&data, 0, 0, 0, approximation);
    }
    
    return 0;
}

void hamiltonian_completion_print(hamiltonian_data_t *data, adjacency_data_t* adj_data)
{
    if(!data || !adj_data || !adj_data->matrix)
    {
        return;
    }
    
    int n = adj_data->n;
    if (0 == data->completion)
    {
        printf("Found Hamiltonian cycle:\n");
        for(int i = 0; i < n; i++)
        {
            printf("%d ", data->examplary_completion[i]);
        }
        printf("\n");
        printf("The number of Hamiltonian cycles: %d\n", data->number_of_completions);
    }
    else
    {

        printf("Size of a minimal Hamiltonian completion: %d\n", data->completion);
        printf("Number of completions: %d\n", data->number_of_completions);
        printf("Example:\n");
        for(int i = 0; i < n; i++)
        {
            int u = data->examplary_completion[i];
            int v = data->examplary_completion[i + 1 == n ? 0 : i + 1];
            if(0 == adj_data->matrix[u * n + v])
            {
                printf("(%d, %d) ", u, v);
            }
        }
        printf("\n");
    }
    fflush(stdout);
}
