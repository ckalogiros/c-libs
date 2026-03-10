/*
 * Created by ckalo on 3/8/26.
 */
#ifndef STRING_STRING_LOG_H
#define STRING_STRING_LOG_H
#include "string_struct.h"

// void
// string_print_uint32(string_print_params);

void
string_print_free_list(const string_t* const string);

void
string_print_char(const string_t* const string);


// ERROR handling
void
string_print_error(const enum STRING_ERROR_CODE code);


#endif //STRING_STRING_LOG_H