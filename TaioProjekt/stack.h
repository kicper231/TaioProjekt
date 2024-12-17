#ifndef STACK_H
#define STACK_H

typedef struct stack {
    int size;
    int* array;
    int count;
} stack_t;

int stack_init(stack_t* stack, int size);
int stack_top(stack_t* stack, int* top);
void stack_pop(stack_t* stack);
void stack_push(stack_t* stack, int value);
void stack_free(stack_t* stack);

#endif