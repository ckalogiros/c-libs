/*
 * Created by ckalo on 1/5/26.
 */

#include "mem_log.h"
#include "mem_man.h"
#include "my_types.h"

uint32 test_id = 0;


int main(void)
{
    memory_manager_init(4);

    mem_log_print_mem(mem_get_mem());
    mem_log_print_free_list(free_list_get_head());
    mem_log_print_all_used_mem();

    memory_manager_terminate();

    return 0;
}

// todo:compare malloc with custom
void name_me_compare_perf_of_malloc_and_mem_manager() {}
