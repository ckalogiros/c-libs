/*
 * Created by ckalo on 1/22/26.
 */


#include "mem_man.h"



int main(void)
{
  memory_manager_init(4);
//
//  mem_log_print_mem(mem_get_mem());
//  mem_log_print_free_list(free_list_get_head());
//  //    mem_log_print_used_mem(used_mem_get_used_mem(), used_mem_get_tail(), used_mem_get_count());
//  mem_log_print_all_used_mem();
//
  memory_manager_terminate();

  return 0;
}
