#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <stdlib.h>

typedef struct union_find 
{
    int n;
    int* sets;
} union_find_t;

// Memory management.
int uf_init(union_find_t* uf, int n);
void uf_free(union_find_t* uf);

// Structure interface.
void uf_reset(union_find_t* uf);
int uf_find(union_find_t* uf, int v);
int uf_union(union_find_t* uf, int u, int v);
int uf_is_intirely_unioned(union_find_t* uf);
void uf_normalize(union_find_t* uf);

#endif