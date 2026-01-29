/*
 * Created by ckalo on 1/6/26.
 */

#include "buffer_log.h"
#include "bits.h"
#include "buffer_internals.h"
#include "logger.h"

#define DEBUG_PRINT_BUFFER
//#undef DEBUG_PRINT_BUFFER // undef to remove print info

void buffer_print_uint32(buffer_print_p params)
{
    #ifdef DEBUG_PRINT_BUFFER

    buffer_t* const buffer = params.buffer;
    const uint32 print_num_elems = params.print_num_elems;

    print("-- %sbuffer%s --\n", ry, CLEAR);
    print(
        "    count:%s%u%s "
        "  tail:%s%d%s "
        "  capacity:%s%u%s\n"
        "    data:%s%p%s "
        "  addr:%s%p%s\n",
        CYAN, buffer->count, CLEAR,
        CYAN, buffer->tail, CLEAR,
        CYAN, buffer->capacity, CLEAR,
        GREEN, REDUCE_ADDR(&(buffer->data), F6), CLEAR,
        GREEN, REDUCE_ADDR(&buffer, F6), CLEAR
    );

    uint64* free_list = buffer->free_list;
    print(" -PRINT free list bitmap\n");
    for (uint32 i = 0; i < buffer->free_list_cnt; i++) {
        bits_print_binary(free_list[i], sizeof(uint64));
    }

    uint32* data = buffer->data;
    print("    ");
    for (uint32 i = 0; i < print_num_elems; i++)
    {
        print(
            "[%s%u%s]=%s%u%s, ",
            YELLOW, i, CLEAR,
            CYAN, data[i], CLEAR
        );

        if ((i+1)%3 == 0) print("\n    ");
    }
    print("\n");

    print("-- %sbuffer%s --\n\n", ry, CLEAR);
    #endif
}

void buffer_print_free_list(const buffer_t * const buffer) {

  uint64* free_list = buffer->free_list;
  print(" -PRINT free list bitmap\n");
  for (uint32 i = 0; i < buffer->free_list_cnt; i++) {
    bits_print_binary(free_list[i], sizeof(uint64));
  }

}

void buffer_print_char(const buffer_t * const buffer) {}



// ERROR handling
void buffer_print_error(const enum BUFFER_ERROR_CODE code)
{
  switch (code) {
  case NO_BUFFER_TABLE_INIT: {
      print("%sERROR:\n"
          "Buffer table is uninitialized\n"
          "Call buffer_init_table()%s\n",
          RED, CLEAR
          );
      break;
    }
  }
}