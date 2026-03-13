/*
 * Created by ckalo on 1/9/26.
 */

#ifndef BUFFER_BUFFER_INTERNALS_H
#define BUFFER_BUFFER_INTERNALS_H

#include "buffer.h"

void TEMP_PRINT_BUFFER_TABLE_COUNT(void);

void buffer_init_table(void);

void buffer_destroy_table(void);

buffer_t* buffer_constructor_uint32(const bufidx_t capacity);

buffer_t* buffer_constructor_gentype(const bufidx_t capacity, const typesize_t typesize);

void buffer_destructor(buffer_t* buffer);

void buffer_release(buffer_t* buffer);

/* PRIVATE */
void buffer_reallocate(buffer_t* const buffer);

void buffer_memcopy(const buffer_t* const dst, const buffer_t* const src);

void buffer_memset(const buffer_t* const buffer, const uint64 val);

void buffer_flist_memset(const buffer_t* const buffer, const uint64 val);


bufidx_t buffer_find_next_free_elem(buffer_t* const buffer);

bufidx_t find_first_free_helper(const uint32 val, const bufidx_t num_elems);

int32 buffer_find_tail_recur(const uint64 val, uint32* accum_num_bits);

bufidx_t TEMP_buffer_find_tail(const buffer_t* const buffer);



/* DEBUG-FPRINT */
void buffer_print_to_file(buffer_t* buffer);
// void* buffer_get_data(const buffer_t* const buffer);
// uint64* buffer_get_freelist(const buffer_t* const buffer);

#endif //BUFFER_BUFFER_INTERNALS_H
