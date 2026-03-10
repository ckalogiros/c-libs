/*
 * Created by ckalo on 3/8/26.
 */

#include "stringc.h"
#include <string.h>
#include "logger.h"
#include "string_internals.h"



void
string_init_table(void)
{
    string_container_init();
}

void
string_destroy_table(void)
{
    string_container_destroy();
}


/***************************************************************************************************
 * CONSTRUCTION-DESTRUCTION
 **************************************************************************************************/
string_t* string_create(const char* str)
{
    return string_constructor(str, 0);
}

string_t* string_create_empty(const strlen_t size)
{
    return string_constructor("", size);
}

void string_destroy(string_t* string)
{
    string_destructor(string);
}


/***************************************************************************************************
 * OPERATIONS
 **************************************************************************************************/
string_t*
string_set_cstr(string_t* string, const char* str)
{
    const strlen_t str_len = strlen(str);
    if(string->size <= str_len) {
        string = string_reallocate(string, str_len);
        // print("STRING REALLOC: new size=%u\n", string->size);
    }
    strlen_t k = 0;
    for(uint32 i = 0; i < str_len; i++) {
        string->data[i] = str[k++];
        string->len++;
    }

    return string;
}

string_t*
string_append_cstr(string_t* string, const char* str)
{
    const strlen_t string_len = string->len;
    const strlen_t str_len = strlen(str);
    // if(string->self_size+string_len+str_len >= string->size) {
    if(string_len+str_len >= string->size-string->self_size) {
        string = string_reallocate(string, str_len);
        // print("STRING REALLOC: new size=%u\n", string->size);
    }

    const strlen_t total_length = string_len + str_len;
    strlen_t k = 0;
    for(uint32 i = string_len; i < total_length; i++) {
        string->data[i] = str[k++];
        string->len++;
    }

    return string;
}

string_t*
string_empty(string_t* string)
{
    memset((void*)string->data, 0, string->len);
    string->len = 0;
    return string;
}
