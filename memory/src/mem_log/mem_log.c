/*
 * Created by ckalo on 1/1/26.
 */

#include "mem_log.h"
#include "logger.h"

#define DEBUG_PRINT_MEM
//#undef DEBUG_PRINT_MEM // undef to remove print info

void
mem_log_print_init_mem()
{
    print("%s#####################\n"
          "  INITIALIZE MEMORY\n"
          "#####################%s\n\n", YELLOW, CLEAR);
}

void
mem_log_print_terminate_mem()
{
    print("\n\n%s#####################\n"
          "  TERMINATE MEMORY\n"
          "#####################%s\n\n", YELLOW, CLEAR);
}

void
mem_log_print_mem()
{
#ifdef DEBUG_PRINT_MEM
    print("-- %smemory block%s --\n", ry, CLEAR);
    print(
        " size bytes: %s%lu%s\n"
        " num pages:  %s%u%s\n"
        " page size:  %s%u%s\n"
        " start  page: %s%x%s\n"
        " last   page: %s%x%s\n"
        " unauth page: %s%x%s\n",
        YELLOW, mem.size, CLEAR,
        YELLOW, mem.num_pages, CLEAR,
        YELLOW, PAGE_SIZE, CLEAR,
        CYAN, REDUCE_ADDR(mem.addr, F6), CLEAR,
        CYAN, REDUCE_ADDR(mem.addr+((mem.num_pages-1)*PAGE_SIZE), F6), CLEAR,
        CYAN, REDUCE_ADDR(mem.addr+mem.size, F6), CLEAR
    );
    print("\n");
#endif
}

void
mem_log_print_all_used_mem()
{
#ifdef DEBUG_PRINT_MEM

    const used_mem_t* used_mem_buf = used_mem_get_used_mem();
    const uint32 count = used_mem_get_count();
    const int32 tail = used_mem_get_tail();

    print("-- %sused mem buf%s --\n", ry, CLEAR);
    print(
        "count:%s%u%s tail:%s%d%s\n",
        CYAN, count, CLEAR,
        CYAN, tail, CLEAR
    );

    print("-- %sused memory%s --\n", ry, CLEAR);
    for(uint32 i = 0; i < tail + 1; i++) {
        print(
            "[%s%u%s] id:%s%u%s num_pages:%s%u%s"
            " addr:%s%x%s\n",
            YELLOW, i & 0xfffffff, CLEAR,
            CYAN, used_mem_buf[i].id, CLEAR,
            CYAN, used_mem_buf[i].num_pages, CLEAR,
            br2, (uint64) (used_mem_buf[i].addr) & F6, CLEAR
        );
    }

    print("-- %sused mem buf%s --\n\n", ry, CLEAR);
#endif
}

void
mem_log_print_used_mem_buf(const char* operation, const used_mem_t* const used_mem)
{
#ifdef DEBUG_PRINT_MEM
    print("-- %sused %s%s", ry, operation, CLEAR);
    print(
        "  id:%s%u%s pages:%s%u%s"
        " addr:%s%x%s\n",
        YELLOW, used_mem->id, CLEAR,
        CYAN, used_mem->num_pages, CLEAR,
        br2, (uint64) (used_mem->addr) & F6, CLEAR
    );
#endif
}

void
mem_log_print_free_list(const free_list_t* const head)
{
#ifdef DEBUG_PRINT_MEM
    print("-- %smem man sys free list%s --\n", ry, CLEAR);
    const free_list_t* node = head;
    uint32 k = 0;
    while(node) {
        // print(
        //     "[%s%u%s] addr:%s%x%s"
        //     " next:%s%x%s\n",
        //     YELLOW, k&0xfffffff, CLEAR,
        //     CYAN, (uint64)(node->addr)&F6, CLEAR,
        //     br2, (uint64)(node->next)&F6, CLEAR
        // );
        node = node->next;
        k++;
    }
    print("free pages:%s%u%s  bytesize:%s%u%s\n", YELLOW, k, CLEAR, YELLOW, k * PAGE_SIZE, CLEAR);
    print("-- %sfree list%s --\n\n", ry, CLEAR);
#endif
}

void
mem_log_print_list_node(const char* operation, const free_list_t* const node)
{
#ifdef DEBUG_PRINT_MEM
    print("-- %slist %s%s", ry, operation, CLEAR);
    print("  page id:%s%u%s addr:%s%x%s\n",
          YELLOW, node->page_id, CLEAR,
          br2, REDUCE_ADDR(node->addr, F6), CLEAR
    );
#endif
}

// ERROR handling
void
mem_print_error(const enum MEMORY_ERROR_CODE code)
{
    switch(code) {
        case NO_MEMORY_MANAGER_INIT: {
            print("%sERROR:\n"
                  "Memory manager is uninitialized\n"
                  "Call memory_manager_init() and don't forget memory_manager_terminate()%s\n",
                  RED, CLEAR
            );
            break;
        }
        case NO_FREE_MEMORY: {
            print("%sERROR:\n"
                  "There is no free memory%s\n",
                  RED, CLEAR
            );
            break;
        }
        case MEMMAN_ALREADY_INITIALIZED: {
            print("%sERROR:\n"
                  "Memory manager already initialized%s\n",
                  RED, CLEAR
            );
            break;
        }
    }
}
