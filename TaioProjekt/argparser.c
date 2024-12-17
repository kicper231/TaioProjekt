#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "getopt.h"

bool parse_flags(int argc, char **argv, bool *metric_flag, bool *size_flag, bool *approx_flag, bool *cycle_flag, bool *hamilton_flag, bool *hamilton_completion_flag, int *return_optind)
{
    int opt;

    while ((opt = getopt(argc, argv, "sramch")) != -1)
    {
        switch (opt)
        {
        case 'm':
            *metric_flag = true;
            break;
        case 's':
            *size_flag = true;
            break;
        case 'a':
            *approx_flag = true;
            break;
        case 'r':
            *hamilton_completion_flag = true;
            break;
        case 'c':
            *cycle_flag = true;
            break;
        case 'h':
            *hamilton_flag = true;
            break;
        case '?':
            fprintf(stderr, "Unrecognized option: '-%c'\n", optopt);
            return false;
        }
    }

    int flag_count = 0;
    flag_count += *metric_flag ? 1 : 0;
    flag_count += *size_flag ? 1 : 0;
    flag_count += *cycle_flag ? 1 : 0;
    flag_count += *hamilton_flag ? 1 : 0;
    flag_count += *hamilton_completion_flag ? 1 : 0;

    if (flag_count != 1)
    {
        ERR("Exactly one of the following flags must be set: -m, -s, -c, -h, -r.");
    }

    if (*metric_flag && argc < 3)
    {
        ERR("It have to be at least 2 files to use -m flag");
    }

    if(*size_flag && *approx_flag)
    {
        ERR("You cant combinated size_flag with approx_flag");
    }

    *return_optind = optind;

    return true;
}