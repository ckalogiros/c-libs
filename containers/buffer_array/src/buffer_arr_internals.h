/*
 * Created by ckalo on 1/9/26.
 */

#ifndef BUFFER_ARR_BUFFER_ARR_INTERNALS_H
#define BUFFER_ARR_BUFFER_ARR_INTERNALS_H

#include "buffer_arr.h"

uint16 buffer_arr_get_allocator_instance_id();

void TEMP_PRINT_BUFFER_ARR_TABLE_COUNT(void);

void buffer_arr_init_table(const pageid_t num_pages);

void buffer_arr_destroy_table(void);

buffer_t* buffer_arr_constructor_uint32(const bufidx_t capacity);

buffer_t* buffer_arr_constructor_gentype(const bufidx_t capacity, const typesize_t typesize);

void buffer_arr_destructor(buffer_t* buffer);

void buffer_arr_release(buffer_t* buffer);

/* PRIVATE */
void buffer_arr_reallocate(buffer_t* const buffer);

void buffer_arr_memcopy(const buffer_t* const dst, const buffer_t* const src);

void buffer_arr_memset(const buffer_t* const buffer, const uint64 val);

void buffer_arr_flist_memset(const buffer_t* const buffer, const uint64 val);


bufidx_t buffer_arr_find_next_free_elem(buffer_t* const buffer);

bufidx_t find_first_free_helper(const uint32 val, const bufidx_t num_elems);

int32 buffer_arr_find_tail_recur(const uint64 val, uint32* accum_num_bits);

bufidx_t TEMP_buffer_arr_find_tail(const buffer_t* const buffer);



/* DEBUG-FPRINT */
uint16 buffer_arr_get_allocator_instance_id();
void buffer_arr_print_to_file(buffer_t* buffer);
// void* buffer_arr_get_data(const bufidx_t* const buffer);
// uint64* buffer_arr_get_freelist(const bufidx_t* const buffer);

#endif //BUFFER_ARR_BUFFER_ARR_NTERNALS_H
