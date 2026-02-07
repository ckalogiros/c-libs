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


struct _IO_FILE;
void
dprint(struct _IO_FILE* file, const uint16 indent, const char* format, ...);

void
dprintmd(struct _IO_FILE* file, const char* dir, const char* filename, uint16 indent, const char* format, ...);


#endif //LOGGER_H