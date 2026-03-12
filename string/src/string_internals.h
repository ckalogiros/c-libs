/*
 * Created by ckalo on 3/8/26.
 */
#ifndef STRING_STRING_INTERNALS_H
#define STRING_STRING_INTERNALS_H
#include "string_struct.h"


void string_container_init(void);

void string_container_destroy(void);

string_t* string_constructor(const char* str, const strlen_t size);

void string_destructor(string_t* string);

void string_release(string_t* string);

string_t* string_reallocate(string_t* string, const strlen_t len, bool copy);


void string_memcopy(string_t* const dst, const string_t* const src);

void string_memset(const string_t* const string, const uint8 val);


#endif //STRING_STRING_INTERNALS_H
