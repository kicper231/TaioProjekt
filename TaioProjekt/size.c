#include "size.h"
#include <math.h>

int size_calculate(graph_t *graph, double *graph_size)
{
    if (!graph || !graph_size)
    {
        return 1;
    }

    int num_vertices = graph->n; 
    int num_edges = 0;         
    double weight_sum = 0.0;   
    int max_weight = 0; 

    for (int i = 0; i < num_vertices; i++)
    {
        struct node *current = graph->edges[i];
        while (current)
        {
            num_edges++;                    
            weight_sum += current->quantity; 
            if (current->quantity > max_weight)
            {
                max_weight = current->quantity; 
            }
            current = current->next; 
        }
    }

    if (num_edges == 0 || max_weight == 0)
    {
        *graph_size = (double)num_vertices;
    }
    else
    {
        double temp_sum = weight_sum / (num_edges * max_weight);
        *graph_size = num_vertices + temp_sum;
    }

    return 0; 
}