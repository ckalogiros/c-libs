/*
 * Created by ckalo on 1/13/26.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "my_types.h"
#include "logger_color.h"


const char* get_color(const char* name);

void print(const char* format, ...);

void print_mem_hex(const uint32 size, char* address);

void print_test(const char* name, const char* format, ...);

void print_implement(const char* function_name, const char* file_name, const uint32 line_number);

void logger_test_variadic(const uint32 args_count, ...);

/* ***************
 * Test printing */
void print_test_result(const char* test_name, bool passed, const char* file, int line, const char* message);


struct _IO_FILE;

void dprint(struct _IO_FILE* file, const uint16 indent, const char* format, ...);

void dprintmd(struct _IO_FILE* file, const char* dir, const char* filename, uint16 indent, const char* format, ...);


#endif //LOGGER_H
