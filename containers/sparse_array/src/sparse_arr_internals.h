/*
 * Created by ckalo on 1/9/26.
 */

#ifndef SPARSE_ARR_SPARSE_ARR_INTERNALS_H
#define SPARSE_ARR_SPARSE_ARR_INTERNALS_H

#include "sparse_arr.h"

uint16 sparse_arr_get_allocator_instance_id();

void TEMP_PRINT_SPARSE_ARR_TABLE_COUNT(void);

void sparse_arr_init_table(const pageid_t num_pages);

void sparse_arr_destroy_table(void);

sparse_arr_t* sparse_arr_constructor_uint32(const bufidx_t capacity);

sparse_arr_t* sparse_arr_constructor_gentype(const bufidx_t capacity, const typesize_t typesize);

void sparse_arr_destructor(sparse_arr_t* buffer);

void sparse_arr_release(sparse_arr_t* buffer);

/* PRIVATE */
void sparse_arr_reallocate(sparse_arr_t* const buffer);

void sparse_arr_memcopy(const sparse_arr_t* const dst, const sparse_arr_t* const src);

void sparse_arr_memset(const sparse_arr_t* const buffer, const uint64 val);

void sparse_arr_flist_memset(const sparse_arr_t* const buffer, const uint64 val);


bufidx_t sparse_arr_find_next_free_elem(sparse_arr_t* const buffer);

bufidx_t find_first_free_helper(const uint32 val, const bufidx_t num_elems);

int32 sparse_arr_find_tail_recur(const uint64 val, uint32* accum_num_bits);

bufidx_t TEMP_sparse_arr_find_tail(const sparse_arr_t* const buffer);



/* DEBUG-FPRINT */
uint16 sparse_arr_get_allocator_instance_id();
void sparse_arr_print_to_file(sparse_arr_t* buffer);
// void* sparse_arr_get_data(const sparse_arr_t* const buffer);
// uint64* sparse_arr_get_freelist(const sparse_arr_t* const buffer);

#endif //SPARSE_ARR_SPARSE_ARR_NTERNALS_H
