//
// Created by ckalo on 12/29/25.
//

#ifndef SPARSE_ARR_SPARSE_ARR_H
#define SPARSE_ARR_SPARSE_ARR_H

#include "sparse_arr_struct.h"
#include "mem_man_types.h"


void sparse_arr_table_init(const pageid_t num_pages);

void sparse_arr_table_destroy(void);

sparse_arr_t* sparse_arr_create_uint32(const uint32 capacity);

sparse_arr_t* sparse_arr_create_gentype(const uint32 capacity, const typesize_t typesize);

void sparse_arr_destroy(sparse_arr_t* buffer);

int32 sparse_arr_push_uint32(const uint32 val, sparse_arr_t* const buffer);

void sparse_arr_insert_uint32(const uint32 val, sparse_arr_t* const buffer, bufidx_t index);

uint16 sparse_arr_set_for_push(sparse_arr_t* const buffer);

void sparse_arr_set_for_insert(sparse_arr_t* const buffer, const bufidx_t index);

void sparse_arr_free_elem(sparse_arr_t* const buffer, const bufidx_t index);

void sparse_arr_empty(sparse_arr_t* const buffer, const bufidx_t index);

uint16 sparse_arr_get_next_free(sparse_arr_t* const buffer);


/* getters */
memid_t sparse_arr_get_memid(sparse_arr_t* const buffer);

/* DEBUG-FPRINT */
void sparse_arr_fprint(sparse_arr_t* buffer);

#endif //SPARSE_ARR_SPARSE_ARR_H
