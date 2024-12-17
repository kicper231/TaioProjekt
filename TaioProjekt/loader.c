#include "loader.h"

#include <stdlib.h>
#include <string.h>

#include "error.h"

char *read_description(FILE *file)
{
    char buffer[1024];
    size_t description_size = 1024;
    char *description = (char *)malloc(description_size * sizeof(char));
    description[0] = '\0';

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strcmp(buffer, "\n") == 0)
        {
            break;
        }

        if (strlen(description) + strlen(buffer) + 1 >= description_size)
        {
            description_size *= 2;
            description = (char *)realloc(description, description_size);
        }
        strcat_s(description, description_size, buffer);
    }

    return description;
}

int file_read(file_t* file, const char *file_name)
{
    int graph_count = 0;

    FILE *open_file = fopen(file_name, "r");

    if (open_file == NULL)
    {
        printf("Could not open the file \"%s\".\n\n", file_name);
        return -1;
    }

    int result = fscanf_s(open_file, "%d", &graph_count);

    if (result == 0)
    {
        printf("Could not read a valid number");
        return -2;
    }
    else if (result == EOF)
    {
        printf("End of the file occurred");
        return -3;
    }
    else if (graph_count == 0)
    {
        printf("Graph_count number cant be 0");
        return -4;
    }

    struct graph **graphs = (struct graph **)malloc((graph_count) * sizeof(struct graph *));

    if(!graphs)
    {
        ERR("Malloc failure!");
    }

    for (int i = 0; i < graph_count; i++)
    {
        int n;
        fscanf_s(open_file, "%d", &n);
        graphs[i] = create_graph(n);

        for (int v = 0; v < n; v++)
        {
            struct node **tail = &(graphs[i]->edges[v]);
            for (int u = 0; u < n; u++)
            {
                int quantity;
                fscanf_s(open_file, "%d", &quantity);
                if (quantity > 0)
                {
                    *tail = create_node(u, quantity);
                    if (!*tail)
                    {
                        ERR("Malloc failure!");
                    }
                    tail = &((*tail)->next);
                }
            }
        }

        fgetc(open_file);
        graphs[i]->description = read_description(open_file);
    }

    file->graph_count = graph_count;
    file->graphs = graphs;
    fclose(open_file);
    
    return 0;
}