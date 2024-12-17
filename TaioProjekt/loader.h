#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include <stdbool.h>

#include "model.h"

typedef struct file
{
    int graph_count;
    struct graph **graphs;
    

} file_t;

char* read_description(FILE* file);
int file_read(file_t* file, const char* filename);

#endif
