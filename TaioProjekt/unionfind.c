#include "unionfind.h"

int uf_init(union_find_t* uf, int n)
{
    // Input validation.
    if(!uf) 
    {
        return 0;
    }
    if(n < 1)
    {
        uf->n = -1;
        uf->sets = NULL;
        return 0;
    }

    // Fields initialization.
    uf->n = n;
    uf->sets = (int*)malloc(n * sizeof(int));

    // Checking if the memory allocation succeeded.
    if(!(uf->sets))
    {
        uf->n = -1;
        uf->sets = NULL;
        return 0;
    }

    // Setting initial array values.
    uf_reset(uf);

    return 1;
}

void uf_free(union_find_t *uf)
{
    if(uf && uf->sets) 
    {
        free(uf->sets);
        uf->sets = NULL;
    }
}

void uf_reset(union_find_t *uf)
{
    if(uf && uf->sets)
    {
        for(int i = 0; i < uf->n; i++)
        {
            uf->sets[i] = i;
        }
    }
}

int uf_find(union_find_t *uf, int v)
{
    // Input validation.
    if(!uf || !(uf->sets) || 
       v >= uf->n || v < 0)
    {
        return -1;
    }

    // Searching for the index of the root of the set.
    int set_index = v;
    while(uf->sets[set_index] != set_index)
    {
        set_index = uf->sets[set_index];
    }

    // Switching pointers to the root of the set.
    int next;
    while(v != set_index)
    {
        next = uf->sets[v];
        uf->sets[v] = set_index;
        v = next;
    }

    // Returning set index.
    return set_index;
}

int uf_union(union_find_t *uf, int u, int v)
{
    // Determining the indexs of the sets.
    int set1 = uf_find(uf, u);
    int set2 = uf_find(uf, v);

    // Indexes validation.   
    if(set1 < 0 || set2 < 0)
    {
        return -1;
    }

    // Making the root of the second set pointing to set1. 
    if(set1 != set2)
    {
        uf->sets[set2] = set1;
    }
    return set1;
}

int uf_is_intirely_unioned(union_find_t *uf)
{
    // Input validation
    if(!uf || !(uf->sets))
    {
        return 0;
    }

    // Iterating the array to check if all individuals 
    // belong to the same set.
    int set0 = uf_find(uf, 0);
    for(int i = 1; i < uf->n; i++)
    {
        if(uf_find(uf, i) != set0)
        {
            // Returning false if i'th set is different than set0.
            return 0;
        }
    }

    return 1;
}

void uf_normalize(union_find_t *uf)
{
    if(uf && uf->sets)
    {
        // Iterating the table and ensuring that 
        // uf->sets[uf->sets[i]] == uf->sets[i].
        // It means that that each individual points
        // to the root of the set that it belongs to.
        for(int i = 0; i < uf->n; i++)
        {
            uf_find(uf, i);
        }
    }
}
