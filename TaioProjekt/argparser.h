#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdbool.h>

bool parse_flags(int argc, char **argv, bool *metric_flag, bool *size_flag, bool *approx_flag, bool *cycle_flag, bool *hamilton_flag, bool *hamilton_count_flag,int *return_optind);

#endif 