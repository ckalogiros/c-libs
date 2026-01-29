/*
 * Created by ckalo on 1/11/26.
 */

#include "buffer_internals.h"
#include "bits.h"
#include "buffer_log.h"
#include "logger.h"
#include "sys_utils.h"
#include "mem_cache.h"

#include <string.h>

#define BUFFER_TABLE_MIN_CAPACITY 32

/*******************************************************************************
 * buffer_table: a buffer to store all applications buffer structures
 *               this is dynamicaly reallocated buffer
 ******************************************************************************/
static buffer_t *buffer_table = NULL; // ptr to memory block
static uint32 buffer_table_count = 0;

struct buffer_table_free_list{
  struct buffer_table_free_list* next;
};

static struct buffer_table_free_list* buffer_table_free_list_head = NULL;
void btable_flist_add(void* ptr) {
  struct buffer_table_free_list* list = ptr;
  list->next = buffer_table_free_list_head;
  buffer_table_free_list_head = list;
}

buffer_t* btable_flist_get_free() {
  struct buffer_table_free_list* list = buffer_table_free_list_head;
  buffer_table_free_list_head = buffer_table_free_list_head->next;
  buffer_table_count++;
  return (buffer_t*)list;
}


void buffer_memcopy(buffer_t *const dst, const buffer_t *const src) {
  memcpy(src->data, src->data, src->capacity*sizeof(uint32));
}
void buffer_memset(const buffer_t* const buffer, const uint64 val) {
  memset(buffer->data, val, buffer->capacity*buffer->typesize);
}

void buffer_flist_memset(const buffer_t* const buffer, const uint64 val) {
  for (uint32 i = 0; i < buffer->free_list_cnt; i++) {
    buffer->free_list[i] = val;
  }
}

void buffer_init_table() {

  if (buffer_table) return;

  const uint32 btable_typesize = sizeof(buffer_t);
  assert(sizeof(buffer_t) > sizeof(void*));

  // assign memory to buffer's free list
  void *const address =
    (buffer_t *)mem_cache_alloc_buffer(BUFFER_TABLE_MIN_CAPACITY, btable_typesize);
  buffer_table = address;

  buffer_t* btable_ptr = buffer_table;
  while ((uint64)btable_ptr < (uint64)(buffer_table + BUFFER_TABLE_MIN_CAPACITY)) {
    btable_flist_add(btable_ptr);
    btable_ptr++;
  }

}


/*
 * constructors-destructors
 */
/*******************************************************************************
 * free_list:  a buffer to store uint64 values as bitmaps
 *                to store free elements of a buffer. the number of
 *                uint64s deppends on the buffer's capacity.
 *             this is dynamicaly reallocated buffer
 *             Ex. for capacity<=64, 1x uint64 as free list,
 *                 for capacity>64 && <=128, 2x uint64 as free list, ...
 ******************************************************************************/
buffer_t *buffer_constructor_uint32(const bufidx_t capacity) {

  if (!buffer_table) {
    buffer_print_error(NO_BUFFER_TABLE_INIT);
    my_exit("buffer_constructor_uint32", __FILE__, __LINE__);
  }

  // how many uint64's do we need to cover all buffer elements. 16 uint64's for buffer capacity=1024
  const uint32 free_list_uint64_count = ((capacity - 1) / BITS64) + 1;

  // mem alloc
  // storage for bufer data
  const void *const data_ptr = mem_cache_alloc_buffer(capacity, sizeof(uint32));
  // storage for buffer free-list data (a uint64 for each 64 buffer elements)
  const uint64 *const buffer_flist_ptr = mem_cache_alloc_buffer(free_list_uint64_count, sizeof(uint64));

  buffer_t *buffer         = btable_flist_get_free();
  buffer->free_list        = buffer_flist_ptr;
  buffer->free_list_cnt    = free_list_uint64_count;
  buffer->data             = (uint32 *)data_ptr;
  buffer->capacity         = capacity;
  buffer->count            = 0;
  buffer->tail             = INT_NULL;
  buffer->id               = 0;
  buffer->typesize         = sizeof(uint32);


  // init buffer data
  buffer_memset(buffer, 0);

  // init buffer free_list
  buffer_flist_memset(buffer, 0xffffffffffffffff);

  return buffer;
}

void buffer_destructor(buffer_t* buffer) {

  void* const ptr = buffer;

  // Release buffer's data slab
  mem_cache_release_buffer(buffer->data, buffer->capacity, sizeof(uint32));

  // Release buffer's free_list slab
  mem_cache_release_buffer(buffer->free_list, buffer->free_list_cnt, sizeof(uint64));

  buffer->data = NULL;
  buffer->free_list = NULL;

  // free buffer from buffer table
  btable_flist_add(ptr);

}


buffer_t *buffer_constructor_gentype(const bufidx_t capacity, const uint16 typesize) {

  if (!buffer_table) {
    buffer_print_error(NO_BUFFER_TABLE_INIT);
    my_exit("buffer_constructor_uint32", __FILE__, __LINE__);
  }

  // how many uint64's do we need to cover all buffer elements. 16 uint64's for buffer capacity=1024
  const uint32 free_list_uint64_count = ((capacity - 1) / BITS64) + 1;

  // mem alloc
  // storage for bufer data
  const void *const data_ptr = mem_cache_alloc_buffer(capacity, typesize);
  // storage for buffer free-list data (a uint64 for each 64 buffer elements)
  const uint64 *const buffer_flist_ptr = mem_cache_alloc_buffer(free_list_uint64_count, sizeof(uint64));

  buffer_t *buffer         = btable_flist_get_free();
  buffer->free_list        = buffer_flist_ptr;
  buffer->free_list_cnt    = free_list_uint64_count;
  buffer->data             = (void *)data_ptr;
  buffer->capacity         = capacity;
  buffer->count            = 0;
  buffer->tail             = INT_NULL;
  buffer->id               = 0;
  buffer->typesize         = typesize;


  // init buffer data
  buffer_memset(buffer, 0);

  // init buffer free_list
  buffer_flist_memset(buffer, 0xffffffffffffffff);

  return buffer;
}


/*
 * helpers
 */
bufidx_t buffer_find_next_free_elem(buffer_t *const buffer) {

  uint64 *free_list = buffer->free_list;

  // run for all possible free_list uint64's
  for (bufidx_t i = 0; i < buffer->free_list_cnt; i++) {

    // check for none 0 uint64 (undefined behavior)
    if (*(free_list+i) != 0) { // if  == 0 then there is no free element
      int32 trailing_zeros = __builtin_ctzll(*(free_list+i));
      // if (trailing_zeros != 0)
        return trailing_zeros; // returns last none zero bit index
    }
  }

  // todo: if no last index was found ???
  my_exit("buffer_find_next_free_elem: NO BUFFER FREE INDEX FOUND", __FILE__, __LINE__);

}

bufidx_t find_first_free_helper(const uint32 val, const bufidx_t num_elems) {
  for (bufidx_t i = 0; i < num_elems; i++) {
    const uint8 bit = bit_is_set(val, i, typeof(val));
    if (bit) { return i; }
  }
  return INT_NULL;
}


/*
 * Find the last element in buffer
 * return: int32 index, -1 error
 */
int32 buffer_find_tail_recur(const uint64 val, uint32 *accum_num_bits) {
  // todo: realloc and/or implement searching to the second uint64 free buffer
  // todo: reimplement with: __builtin_ctzll();

  uint64 low       = bits_mask_low(val);
  uint32 type_size = type_size_bits(val);

  int32 index = INT_NULL;
  if (low != 0) {
    index = find_first_free_helper(low, type_size);
  } else {
    // on every recursion, cut type size by half, and accumulate all the previous devisions lower part
    *accum_num_bits += half_type_bits(low);
    uint64 high     = bits_mask_high(val);
    index           = buffer_find_tail_recur(high, accum_num_bits);
  }

  return index;
}

bufidx_t TEMP_buffer_find_tail(const buffer_t *const buffer) {
  uint32 accum_num_bits = 0;
  uint64 free_list      = *buffer->free_list;
  // todo: free_list is a buffer of uint64
  const bufidx_t index = buffer_find_tail_recur(free_list, &accum_num_bits);
  print("accum_num_bits = %d\n", accum_num_bits);

  if (index == INT_NULL) {
    // no free index found
    // todo: realloc and/or implement searching to the second uint64 free buffer
    my_exit("index == INT_NULL", __FILE__, __LINE__);
  }

  return index + accum_num_bits;
}



/*
 * private
 */
void buffer_reallocate(buffer_t *const buffer) {

  // 1. allocate new buffer
  // 2. copy data
  // 3, release old buffer data

  // 4. realloc buffers's free_list ptr
  // 5. release old buffer's free_list ptr

  const bufidx_t new_capacity = buffer->capacity * 2;

  // 1.
  void *const data_ptr   = (uint32*)mem_cache_alloc_buffer(new_capacity, sizeof(uint32));

  // 2.
  memcpy(data_ptr, buffer->data, buffer->capacity*sizeof(uint32));

  // 3.
  mem_cache_release_buffer(buffer->data, buffer->capacity, sizeof(uint32));

  // 4.
  const uint32 free_list_count = ((buffer->capacity - 1) / BITS64) + 1;
  void *const free_list_ptr   = (uint64*)mem_cache_alloc_buffer(free_list_count, sizeof(uint64));

  // 5.
  mem_cache_release_buffer(buffer->free_list, free_list_count, sizeof(uint64));

  buffer->data = data_ptr;
  buffer->free_list = free_list_ptr;
  buffer->capacity = new_capacity;

}

// todo: create generi type realloc()