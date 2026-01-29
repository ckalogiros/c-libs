/*
 * Created by ckalo on 1/9/26.
 */

#ifndef BUFFER_BUFFER_INTERNALS_H
#define BUFFER_BUFFER_INTERNALS_H

#include "buffer.h"


void buffer_init_table();

buffer_t* buffer_constructor_uint32(const bufidx_t capacity);
// buffer_t* buffer_constructor_vec4f(const bufidx_t capacity);
// buffer_t* buffer_constructor_vec3f(const bufidx_t capacity);
// buffer_t* buffer_constructor_vec2f(const bufidx_t capacity);
buffer_t *buffer_constructor_gentype(const bufidx_t capacity, const uint16 typesize);
void buffer_destructor(buffer_t * buffer);

void buffer_reallocate(buffer_t *const buffer);


bufidx_t buffer_find_next_free_elem(buffer_t *const buffer);
bufidx_t find_first_free_helper(const uint32 val, const bufidx_t num_elems);
int32 buffer_find_tail_recur(const uint64 val, uint32* accum_num_bits);
bufidx_t TEMP_buffer_find_tail(const buffer_t *const buffer);

#endif //BUFFER_BUFFER_INTERNALS_H

