#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>

void timer_init(); 
void timer_start();
void timer_stop();
double timer_elapsed_seconds();

#define TIMER(instructions, description) \
        timer_start(); \
        instructions; \
        timer_stop(); \
        printf("%s took %E seconds to execute.\n", description, timer_elapsed_seconds());
#endif