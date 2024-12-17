#include "timer.h"

#include <time.h>
#include <stdbool.h>

#ifdef _WIN64
#include <windows.h>

LARGE_INTEGER frequency;
LARGE_INTEGER start;
LARGE_INTEGER end;

double interval;
#else
clock_t timer;
#endif 

void timer_init()
{
#ifdef _WIN64
    QueryPerformanceFrequency(&frequency);
#endif
}


void timer_start()
{
#ifdef _WIN64   
    QueryPerformanceCounter(&start);
#else
    timer = clock();
#endif 
}

void timer_stop()
{
#ifdef _WIN64
    QueryPerformanceCounter(&end);
#else
    timer = clock() - timer;
#endif 
    
}

double timer_elapsed_seconds()
{
#ifdef _WIN64
    return (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
#else
    return ((double)timer)/CLOCKS_PER_SEC;
#endif 
}