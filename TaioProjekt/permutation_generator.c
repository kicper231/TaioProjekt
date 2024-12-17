#include "generators.h"

#include <stdlib.h>
#include <time.h>

#define LEFT_TO_RIGHT 1
#define RIGHT_TO_LEFT 0

int permutation_generator_init(permutation_generator_t* generator, int n)
{
    if(!generator || n < 1)
    {
        return 1;
    }

    generator->n = n;
    generator->list = (int*)malloc(n * sizeof(int));
    if(!generator->list)
    {
        return -1;
    }
    permutation_generator_reset(generator);
    return 0;
}

void permutation_generator_reset(permutation_generator_t* generator)
{
    if(!generator || !generator->list)
    {
        return;
    }
    for(int i = 0; i < generator->n; i++)
    {
        generator->list[i] = i + 1 < generator->n ? i + 1 : 0;
    }
}

void permutation_generator_free(permutation_generator_t *generator)
{
    if(generator && generator->list)
    {
        free(generator->list);
    }
}

void permutation_generator_generate(permutation_generator_t *generator, int *result)
{
    if(!generator || !result)
    {
        return;
    }

    srand(time(NULL));
    int range = generator->n;
    int prev = generator->n - 1;
    int current = 0;
    for(int i = 0; i < generator->n; i++)
    {
        int steps = rand() % range--;
        while(steps--)
        {
            prev = current;
            current = generator->list[current];
        } 
        result[i] = current;
        current = generator->list[current];
        generator->list[prev] = current;
    }
}

int permutation_jta_generator_init(permutation_jta_data_t *data, int n)
{
    if(!data || n < 1)
    {
        return 1;
    }

    data->n = n;
    data->directions = (int*) malloc(n * sizeof(int));
    if(!data->directions)
    {
        return -1;
    }

    data->list = (int*) malloc(n * sizeof(int));
    if (!data->list)
    {
        permutation_jta_generator_free(data);
        return -2;
    }

    int reset_result =  permutation_jta_generator_reset(data);

    if(0 != reset_result)
    {
        permutation_jta_generator_free(data);
        return -3;
    }

    return 0;
}

int permutation_jta_generator_reset(permutation_jta_data_t *data)
{
    if (!data || data->n < 1 || !data->directions || !data->list)
    {
        return 1;
    }

    for (int i = 0; i < data->n; i++)
    {
        data->list[i] = i;
        data->directions[i] = 0;
    }
    
    return 0;
}
void permutation_jta_generator_free(permutation_jta_data_t *data){

    if (data)
    {
        if(data->directions)
        {
            free(data->directions);
        }
        if(data->list)
        {
            free(data->list);
        }
    }
}

int permutation_jta_generator_generate(permutation_jta_data_t *data, int *result)
{
    int mobile_pos = permutation_jta_generator_mobile_pos(data);
    int mobile = data->list[mobile_pos];

    // swapping the elements according to the
    // data->directionsection i.e. data->directions[].
    if (data->directions[mobile - 1] == RIGHT_TO_LEFT)
        swap(&data->list[mobile_pos], &data->list[mobile_pos - 1]);

    else if (data->directions[mobile - 1] == LEFT_TO_RIGHT)
        swap(&data->list[mobile_pos], &data->list[mobile_pos + 1]);

    // changing the data->directionsections for elements
    // greater than largest mobile integer.
    for (int i = 0; i < data->n; i++)
    {
        if (data->list[i] > mobile)
        {
            if (data->directions[data->list[i] - 1] == LEFT_TO_RIGHT)
                data->directions[data->list[i] - 1] = RIGHT_TO_LEFT;
            else if (data->directions[data->list[i] - 1] == RIGHT_TO_LEFT)
                data->directions[data->list[i] - 1] = LEFT_TO_RIGHT;
        }
    }

    for(int i = 0; i < data->n; i++)
    {
        result[i] = data->list[i];
    }
    
    return 0;
}

int permutation_jta_generator_mobile_pos(permutation_jta_data_t *data)
{
    int  mobile_pos = 0; 
    int  mobile = 0;
    for (int i = 0; i < data->n; i++)
    {
        if (i != 0 && data->directions[data->list[i] - 1] == RIGHT_TO_LEFT )
        {
            if (data->list[i] > data->list[i - 1] && data->list[i] > mobile)
            {
                mobile_pos = i;
                mobile = data->list[mobile_pos];
            }
        }

        if (i != data->n - 1 && data->directions[data->list[i] - 1] == LEFT_TO_RIGHT)
        {
            if (data->list[i] > data->list[i + 1] && data->list[i] > mobile)
            {
                mobile_pos = i;
                mobile = data->list[mobile_pos];
            }
        }
    }

    return mobile_pos;
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}