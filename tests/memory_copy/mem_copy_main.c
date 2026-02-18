/*
 * Created by ckalo on 1/27/26.
 */

#include "buffer_internals.h"
#include "buffer_struct.h"
#include "logger.h"
#include "mem_man_interface.h"

#define NUM_PAGES 10
#define CHAR_NUM_ELEMENTS (NUM_PAGES * PAGE_SIZE)

buffer_t* buffer;


int main(void)
{
  memman_init();
  buffer_init_table();

  buffer = buffer_create_gentype(CHAR_NUM_ELEMENTS, sizeof(uint8));

  for (uint32 i = 0; i < CHAR_NUM_ELEMENTS; i++) {

    buffer_set_for_push(buffer);
    uint8* data = buffer->data;
    data[i] = i;
  }

  for (uint32 i = 0; i < CHAR_NUM_ELEMENTS; i++) {

    uint8* data = buffer->data;
    print("[%u] = %u\n", i, data[i]);
  }

  // test_ecs();

  memman_terminate();

  return 0;
}