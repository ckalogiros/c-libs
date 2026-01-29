//
// Created by ckalo on 12/29/25.
//

#include "buffer.h"
#include "buffer_internals.h"
#include "sys_utils.h"
#include "bits.h"

#include <string.h>

#define BUFFER_DEBUG_PRINT 1
#undef BUFFER_DEBUG_PRINT

#define buffer_free_buffer_insert(value, bit) (bit_set(value, bit, uint64))
#define buffer_free_buffer_remove(value, bit) (bit_clear(value, bit, uint64))



void buffer_init() {
  buffer_init_table();
}

buffer_t* buffer_create_uint32(const uint32 capacity)
{ return buffer_constructor_uint32(capacity); }
// buffer_t* buffer_create_vec4f(const uint32 capacity)
// { return buffer_constructor_vec4f(capacity); }
// buffer_t* buffer_create_vec3f(const uint32 capacity)
// { return buffer_constructor_vec3f(capacity); }
// buffer_t* buffer_create_vec2f(const uint32 capacity)
// { return buffer_constructor_vec2f(capacity); }
buffer_t* buffer_create_gentype(const uint32 capacity, const uint32 typesize)
{ return buffer_constructor_gentype(capacity, typesize); }

void buffer_destroy(buffer_t* buffer) {

  buffer_destructor(buffer);
}

int32 buffer_push_uint32(const uint32 val, buffer_t *const buffer)
{
    // todo: implement realloc
    if (buffer->tail+1 >= buffer->capacity) {
      buffer_reallocate(buffer);
    }
    else
    {
        buffer->tail++;
        uint32* data = buffer->data;
        data[buffer->tail] = val;
        buffer->count++;

        const uint32 offset = buffer->tail / BITS64;
        uint64* free_list = buffer->free_list+offset;
        buffer_free_buffer_remove(*free_list, index);

      return buffer->tail;

    }

  return INT_NULL;
}
// uint32 buffer_push_vec4f(const vec4f val, buffer_t *const buffer)
// {
//   // todo: implement realloc
//   if (buffer->tail+1 >= buffer->capacity) {
//     buffer_reallocate(buffer);
//   }
//   else
//   {
//     buffer->tail++;
//     vec4f* data = buffer->data;
//     data[buffer->tail] = val;
//     buffer->count++;
//     uint64* const free_list = buffer_get_free_list_with_offset(buffer->free_list_idx, buffer->tail);
//     buffer_free_buffer_remove(*free_list, buffer->tail);
//     return buffer->tail;
//
//   }
//
//   return INT_NULL;
// }
// uint32 buffer_push_vec3f(const vec3f val, buffer_t *const buffer)
// {
//   // todo: implement realloc
//   if (buffer->tail+1 >= buffer->capacity) {
//     buffer_reallocate(buffer);
//   }
//   else
//   {
//     buffer->tail++;
//     vec3f* data = buffer->data;
//     data[buffer->tail] = val;
//     buffer->count++;
//     uint64* const free_list = buffer_get_free_list_with_offset(buffer->free_list_idx, buffer->tail);
//     buffer_free_buffer_remove(*free_list, buffer->tail);
//     return buffer->tail;
//
//   }
//
//   return INT_NULL;
// }
// void buffer_push_vec2f(const vec2f val, buffer_t *const buffer)
// {
//   // todo: implement realloc
//   if (buffer->tail+1 >= buffer->capacity) {
//     buffer_reallocate(buffer);
//   }
//   else
//   {
//     buffer->tail++;
//     vec2f* data = buffer->data;
//     data[buffer->tail] = val;
//     buffer->count++;
//     uint64* const free_list = buffer_get_free_list_with_offset(buffer->free_list_idx, buffer->tail);
//     buffer_free_buffer_remove(*free_list, buffer->tail);
//
//   }
// }
// uint32 buffer_push_gentype(const void* const val, buffer_t *const buffer)
// {
//   // todo: implement realloc
//   if (buffer->tail+1 >= buffer->capacity) {
//     my_exit("IMPLEMENT REALLOC() FOR buffer_push_gentype", __FILE__, __LINE__);
//     buffer_reallocate(buffer);
//   }
//   else
//   {
//     buffer->tail++;
//
//     uint8* ptr = (uint8*) buffer->data; // cast FIRST
//     const uint32 index = buffer->tail * buffer->typesize;
//     // todo: INEFFICIENT!. create custom memcpy, OR ??
//     memcpy(ptr+index, val, buffer->typesize);
//
//     buffer->count++;
//     // uint64* const free_list = buffer_get_free_list_with_offset(buffer->free_list_idx, buffer->tail);
//     const bufidx_t offset = index / BITS64;
//     uint64 free_list = buffer->free_list[offset];
//     buffer_free_buffer_remove(free_list, buffer->tail);
//
//     return buffer->tail;
//
//   }
//
//   return INT_NULL;
// }

void buffer_insert_uint32(const uint32 val, buffer_t *const buffer, const bufidx_t index)
{
  // todo: implement realloc
  if (buffer->tail+1 > buffer->capacity){ my_exit("buffer-implement realloc", __FILE__, __LINE__); }
  else
  {
    uint32* data = buffer->data;
    data[index] = val;
    buffer->count++; // todo: we assume that we insert in a none occupied element
    // increment tail only if index is greater
    if (index > buffer->tail) buffer->tail = index;

    const uint32 offset = index / BITS64;
    uint64* free_list = buffer->free_list+offset;
    buffer_free_buffer_remove(*free_list, index);

  }
}

void buffer_remove_uint32(buffer_t *const buffer, const bufidx_t index)
{
  if (index >= buffer->capacity){ my_exit("buffer-implement realloc", __FILE__, __LINE__); }
  else
  {
    uint32* data = buffer->data;
    data[index] = 0;
    buffer->count--;
    /*
     *todo
     * if count is < tail+1, then we need to tail-- until first occupied elem is found
     */
    if (index == buffer->tail) buffer->tail--;

    const uint32 offset = index / BITS64;
    uint64* free_list = buffer->free_list+offset;
    buffer_free_buffer_insert(*free_list, index);
  }
}
// void buffer_remove_vec4f(buffer_t *const buffer, const bufidx_t index)
// {
//   if (index >= buffer->capacity){ my_exit("buffer-implement realloc", __FILE__, __LINE__); }
//   else
//   {
//     vec4f* data = buffer->data;
//     data[index] = (vec4f){0};
//     buffer->count--;
//     /*
//      *todo
//      * if count is < tail+1, then we need to tail-- until first occupied elem is found
//      */
//     if (index == buffer->tail) buffer->tail--;
//     uint64* free_list = buffer_get_free_list_with_offset(buffer->free_list_idx, index);
//     buffer_free_buffer_insert(*free_list, index);
//   }
// }
// void buffer_remove_vec3f(buffer_t *const buffer, const bufidx_t index)
// {
//   if (index >= buffer->capacity){ my_exit("buffer-implement realloc", __FILE__, __LINE__); }
//   else
//   {
//     vec3f* data = buffer->data;
//     data[index] = (vec3f){0};
//     buffer->count--;
//     /*
//      *todo
//      * if count is < tail+1, then we need to tail-- until first occupied elem is found
//      */
//     if (index == buffer->tail) buffer->tail--;
//     uint64* free_list = buffer_get_free_list_with_offset(buffer->free_list_idx, index);
//     buffer_free_buffer_insert(*free_list, index);
//   }
// }
// void buffer_remove_vec2f(buffer_t *const buffer, const bufidx_t index)
// {
//   if (index >= buffer->capacity){ my_exit("buffer-implement realloc", __FILE__, __LINE__); }
//   else
//   {
//     vec2f* data = buffer->data;
//     data[index] = (vec2f){0};
//     buffer->count--;
//     /*
//      *todo
//      * if count is < tail+1, then we need to tail-- until first occupied elem is found
//      */
//     if (index == buffer->tail) buffer->tail--;
//     uint64* free_list = buffer_get_free_list_with_offset(buffer->free_list_idx, index);
//     buffer_free_buffer_insert(*free_list, index);
//   }
// }

/*
 * getters
 */
uint16 buffer_get_next_free(buffer_t* const buffer) {
  const uint16 first_free_index = buffer_find_next_free_elem(buffer);
  return first_free_index;
}



uint16 buffer_set_for_push(buffer_t *const buffer) {
  // todo: implement realloc
  if (buffer->tail+1 >= buffer->capacity) {
    buffer_reallocate(buffer);
    my_exit("ERROR in buffer_set_for_push().\n"
            "buffer_reallocate deals with uint32 buffer type\n"
            "but buffer_set_for_push is a generic type function\n"
            "SO REALLOC DOES  NOT WORK!!!",
            __FILE__, __LINE__);
  }
  else
  {
    buffer->tail++;
    buffer->count++;
    const bufidx_t offset = buffer->tail / BITS64;
    uint64* free_list = &buffer->free_list[offset];
    buffer_free_buffer_remove(*free_list, buffer->tail);

    return buffer->tail;

  }

  return INT_NULL;
}

void buffer_set_for_insert(buffer_t *const buffer, const bufidx_t index)
{
  // todo: implement realloc
  if (buffer->tail+1 > buffer->capacity){ my_exit("buffer-implement realloc", __FILE__, __LINE__); }
  else
  {
    buffer->count++; // todo: we assume that we insert in a none occupied element
    // increment tail only if index is greater
    if (index > buffer->tail) buffer->tail = index;

    const uint32 offset = index / BITS64;
    uint64* free_list = buffer->free_list+offset;
    buffer_free_buffer_remove(*free_list, index);

  }
}
