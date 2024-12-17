#include "model.h"

#include <stdio.h>
#include <stdlib.h>

struct node* create_node(int v, int quantity)
{
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->vertex = v;
    new_node->quantity = quantity;
    new_node->next = NULL;
    return new_node;
}

struct graph* create_graph(int n)
{
    struct graph* graph = (struct graph*)malloc(sizeof(struct graph));
    graph->edges = (struct node**)malloc(n * sizeof(struct node*));
    graph->description = NULL;
    graph->n = n;

    for (int v = 0; v < n; v++)
    {
        graph->edges[v] = NULL;
    }
    return graph;
}

void add_edge(struct graph* graph, int from, int to, int quantity)
{
    if (quantity > 0)
    {
        struct node* new_node = create_node(to, quantity);
        new_node->next = graph->edges[from];
        graph->edges[from] = new_node;
    }
}

void print_graph(struct graph* graph)
{
    if(!graph)
    {
        return;
    }

    printf("Graph n=%d\n", graph->n);
    for (int v = 0; v < graph->n; v++)
    {
        struct node* node = graph->edges[v];
        printf("%d -> ", v);

        if (node)
        {
            printf("%d(%d)", node->vertex, node->quantity);
            node = node->next;
        }
        while (node)
        {
            printf(", %d(%d)", node->vertex, node->quantity);
            node = node->next;
        }
        printf("\n");
    }

    if (graph->description)
    {
        printf("Description:\n%s", graph->description);
    }
    printf("\n");
}

void free_graph(struct graph* graph)
{
    for (int v = 0; v < graph->n; v++)
    {
        struct node* node = graph->edges[v];

        while (node != NULL)
        {
            struct node* node_to_remove = node;
            node = node->next;
            free(node_to_remove);
        }
    }

    free(graph->edges);

    if (graph->description)
    {
        free(graph->description);
    }

    free(graph);
}

void free_all_graphs(struct graph** graphs, int graph_count)
{

    if(!graphs)
    {
        return;
    }

    for (int i = 0; i < graph_count; i++)
    {
        free_graph(graphs[i]);
    }

    free(graphs);
}