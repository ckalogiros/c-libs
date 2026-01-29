/*
 * Created by ckalo on 1/28/26.
 */

#include "buffer_impl.h"
#include "buffer.h"

#include <assert.h>


uint32 buffer_push_timer(struct timer t, buffer_t *const buffer)
{

  const uint32 index = buffer_set_for_push(buffer);
  struct timer* data = buffer->data;
  data[buffer->tail] = t;

  // assert(index == INT_NULL);

  return index;
}

void buffer_insert_timer(struct timer t, buffer_t* const buffer, bufidx_t index) {

  buffer_set_for_insert(buffer, index);
  struct timer* data = buffer->data;
  data[index] = t;
}