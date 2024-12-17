#ifndef GENERATORS_H
#define GENERATORS_H

typedef struct permutation_generator {
    int n;
    int* list;
} permutation_generator_t;

typedef struct permutation_jta_data
{
    int* list;
    int* directions;
    int n;
} permutation_jta_data_t;

int permutation_generator_init(permutation_generator_t *generator, int n);
void permutation_generator_reset(permutation_generator_t* generator);
void permutation_generator_free(permutation_generator_t* generator);
void permutation_generator_generate(permutation_generator_t* generator, int* result);

int permutation_jta_generator_init(permutation_jta_data_t *generator_data, int n);
int permutation_jta_generator_reset(permutation_jta_data_t *generator_data);
void permutation_jta_generator_free(permutation_jta_data_t *generator_data);
int permutation_jta_generator_generate(permutation_jta_data_t *generator_data, int* result);

int permutation_jta_generator_mobile_pos(permutation_jta_data_t *data);
void swap(int *a, int *b);

#endif