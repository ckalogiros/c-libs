/*
 * Created by ckalo on 1/13/26.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "my_types.h"
#include "logger_color.h"


const char *get_color(const char *name);
void print(const char *format, ...);
void logger_test_variadic(const uint32 args_count, ...);
void logger_print_sample1();

#endif //LOGGER_H