#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

#include "error.h"

static int stack_expand(stack_t* stack);

int stack_init(stack_t *stack, int n)
{
    if(!stack || n < 1) 
    {
        return 1;
    }

    stack->size = n;
    stack->count = 0;
    stack->array = (int*)malloc(sizeof(int) * n);

    return NULL == stack->array ? -1 : 0;
}

int stack_top(stack_t *stack, int* top)
{
    if(!stack || !stack->array || !stack->count || !top)
    {
        return 1;
    }

    *top = stack->array[stack->count - 1];
    return 0;
}

void stack_pop(stack_t *stack)
{
    if(stack->count > 0)
    {
        stack->count--;
    }
}

void stack_push(stack_t *stack, int value)
{
    if(!stack || !stack->array)
    {
        return;
    }

    if(stack->size == stack->count && 0 != stack_expand(stack))
    {
        ERR("Could not resize the stack!");
    }  

    stack->array[stack->count++] = value;  
}

void stack_free(stack_t* stack)
{
    if(stack && stack->array) 
    {
        free(stack->array);
        stack->array = NULL;
    }
}

int stack_expand(stack_t* stack) 
{
    if(!stack || !stack->array)
    {
        ERR("Invalid input!");
    }

    int* new_array = realloc(stack->array, 2 * sizeof(int) * stack->size);
    if(!new_array) 
    {
        return -1;
    }

    stack->size *= 2;
    stack->array = new_array;
    return 0;
}
