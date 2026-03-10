/*
 * Created by ckalo on 3/8/26.
 */
#ifndef STRING_STRING_H
#define STRING_STRING_H


#include "string_struct.h"


void string_init_table(void);

void string_destroy_table(void);


/* CONSTRUCTION-DESTRUCTION */
string_t* string_create(const char* str);

string_t* string_create_empty(const strlen_t len);

void string_destroy(string_t* string);

/* OPERATIONS */
string_t* string_set_cstr(string_t* string, const char* str);

string_t* string_append_cstr(string_t* const string, const char* str);

string_t* string_empty(string_t* string);

// void string_push(string_t* string);


// int32 string_push_uint32(const uint32 val, string_t* const string);
//
// void string_insert_uint32(const uint32 val, string_t* const string, bufidx_t index);
//
// uint16 string_set_for_push(string_t* const string);
//
// void string_set_for_insert(string_t* const string, const bufidx_t index);
//
// void string_free_elem(string_t* const string, const bufidx_t index);
//
// void string_empty(string_t* const string, const bufidx_t index);
//
// /* getters */
// uint16 string_get_next_free(string_t* const string);
//
// /* DEBUG-FPRINT */
// void string_fprint(string_t* string);

#endif //STRING_STRING_H