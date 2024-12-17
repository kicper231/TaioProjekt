#ifndef MODEL_H
#define MODEL_H

typedef struct node
{
    int vertex;
    int quantity;
    struct node* next;
} node_t;

typedef struct graph
{
    int n;
    struct node** edges;
    char* description;
} graph_t;


struct node* create_node(int v, int quantity);
struct graph* create_graph(int n);
void add_edge(struct graph* graph, int from, int to, int quantity);
void print_graph(struct graph* graph);
void free_graph(struct graph* graph);
void free_all_graphs(struct graph** graphs, int graph_count);

#endif