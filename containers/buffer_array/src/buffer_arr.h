//
// Created by ckalo on 12/29/25.
//

#ifndef BUFFER_ARR_BUFFER_ARR_H
#define BUFFER_ARR_BUFFER_ARR_H

#include "buffer_arr_struct.h"
#include "mem_man_types.h"


void buffer_arr_table_init(const pageid_t num_pages);

void buffer_arr_table_destroy(void);

buffer_t* buffer_arr_create_uint32(const uint32 capacity);

buffer_t* buffer_arr_create_gentype(const uint32 capacity, const typesize_t typesize);

void buffer_arr_destroy(buffer_t* buffer);

int32 buffer_arr_push_uint32(const uint32 val, buffer_t* const buffer);

void buffer_arr_insert_uint32(const uint32 val, buffer_t* const buffer, bufidx_t index);

int16 buffer_arr_set_for_push(buffer_t* const buffer);

void buffer_arr_set_for_insert(buffer_t* const buffer, const bufidx_t index);

void buffer_arr_free_elem(buffer_t* const buffer, const bufidx_t index);

void buffer_arr_empty(buffer_t* const buffer, const bufidx_t index);

int16 buffer_arr_get_next_free(buffer_t* const buffer);


/* getters */
memid_t buffer_arr_get_memid(buffer_t* const buffer);

/* DEBUG-FPRINT */
void buffer_arr_fprint(buffer_t* buffer);

#endif //BUFFER_ARR_BUFFER_ARR_H
