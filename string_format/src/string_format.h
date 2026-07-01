/*
 * Created by ckalo on 3/9/26.
 */
#ifndef STRING_FORMAT_H
#define STRING_FORMAT_H
#include "my_types.h"
#include <stdarg.h>




uint32 string_format(char** const buf, const uint32 size, const char* format, ...);

uint32 string_format_va_list_call(char** const buf, const uint32 size, const char* format, va_list args);

uint32 string_buffer_push(char** const buf, const uint32 size, const char* str);

void   string_format_test();


#endif //STRING_FORMAT_H