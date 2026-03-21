/*
 * Created by ckalo on 1/1/26.
 */

#include "mem_log.h"
#include "mem_man_structs.h"
#include "mem_man_interface.h"
#include "logger.h"
#include "sys_utils.h"


#define DEBUG_PRINT_MEM
// #undef DEBUG_PRINT_MEM // undef to remove print info
#define DEBUG_HTML_DIR "/data/projects/clion/memory/memory_debug.html"



void
mem_log_print_init_mem(void)
{
#ifdef DEBUG_PRINT_MEM
    print("%s# INITIALIZE MEMORY #%s\n\n", YELLOW, CLEAR);
#endif
}

void
mem_log_print_terminate_mem(void)
{
#ifdef DEBUG_PRINT_MEM
    print("\n\n%s# TERMINATE MEMORY #%s\n", YELLOW, CLEAR);
#endif
}

void
mem_log_print_mem(const struct mem_block* const mem)
{
#ifdef DEBUG_PRINT_MEM
    print("-- %smemory%s --\n", ry, CLEAR);
    print(
        " size bytes:  %s%lu, %lukb%s\n"
        " data size:   %s%lu, %lukb%s\n"
        " total pages: %s%u%s\n"
        " mem_block start:      %s%x%s\n"
        " data start:     %s%x%s\n"
        " last page:      %s%x%s\n"
        " unauth page:    %s%x%s\n",
        YELLOW, mem->size, mem->size/PAGE_SIZE, CLEAR,
        YELLOW, mem->data_size, mem->data_size/PAGE_SIZE, CLEAR,
        YELLOW, mem->num_pages, CLEAR,
        CYAN, REDUCE_ADDR((char*)mem->start, F6), CLEAR,
        CYAN, REDUCE_ADDR((char*)mem->start + mem->data_size, F6), CLEAR,
        CYAN, REDUCE_ADDR((char*)mem->start + mem->size - mem->poison_data_size, F6), CLEAR,
        CYAN, REDUCE_ADDR((char*)mem->start + mem->size, F6), CLEAR
    );


    print("\n");
#endif
}

void
mem_log_print_free_list(const memid_t memid)
{
#ifdef DEBUG_PRINT_MEM
    print("-- %sfree list heads table%s --\n", ry, CLEAR);

    struct flist* flist = mem_table_get_head(memid);

    if(!flist)
        print("head = %sNULL%s\n", YELLOW, CLEAR);

    uint32 i = 0;
    while(flist) {
        print(
            "[%s%u%s]"
            " addr:%s%x%s\n",
            CYAN, i, CLEAR,
            CYAN, (uint64)REDUCE_ADDR(flist,F6), CLEAR);
        flist = flist->next;
        i++;
    }
#endif
}

void
mem_print_mem_alloc(void* allocated_addr, const struct memhnd_t hnd)
{
    print("%smem_alloc()%s"
      " page:%s%u%s"
      " addr:%s%x%s"
      " nextFree:%s%x%s\n",
      ry, CLEAR,
      YELLOW, hnd.page_id, CLEAR,
      br1, allocated_addr, CLEAR,
      br2, mem_table_get_head(hnd.mem_id), CLEAR
      );
}

void
mem_print_mem_release(void* released_addr, const struct memhnd_t hnd)
{
    print("%smem_release()%s"
          " page:%s%u%s"
          " addr:%s%x%s"
          " head:%s%x%s\n",
          ry, CLEAR,
          YELLOW, hnd.page_id, CLEAR,
          br1, released_addr, CLEAR,
          br2, mem_table_get_head(hnd.mem_id), CLEAR
          );
}

// ERROR handling
void
mem_print_error(const enum MEMORY_ERROR_CODE code)
{
    switch(code) {
        case MALLOC_FAILED_TO_ALLOC_MEMORY: {
            print("%sERROR:\n"
                  "Aligned_alloc() FAILED to allocate memory%s\n",
                  RED, CLEAR
            );
            break;
        }
        case NO_MEMORY_MANAGER_INIT: {
            print("%sERROR:\n"
                  "Memory manager is uninitialized\n"
                  "Call memman_init() and don't forget memman_terminate()%s\n",
                  RED, CLEAR
            );
            break;
        }
        case NO_FREE_MEMORY: {
            print("%sERROR:\n"
                  "There is no free memory%s\n",
                  RED, CLEAR
            );
            // my_exit("NO FREE MEMORY", __FILE__, __LINE__);
            break;
        }
        case MEMMAN_ALREADY_INITIALIZED: {
            print("%sERROR:\n"
                  "Memory manager already initialized%s\n",
                  RED, CLEAR
            );
            break;
        }
        case ALLOC_NOT_CONTIGUOUS_MEM: {
            print("%sERROR:\n"
                  "Trying to allocate not contiguous free memory%s\n",
                  RED, CLEAR
            );
            break;
        }
        case MEMORY_IS_IN_USE: {
            print("%sERROR:\n"
                  "Trying to release memory that has pages\n"
                  "so it is considered to be in use%s\n",
                  RED, CLEAR
            );
            break;
        }
        case FREE_MEMORY_FAILED: {
            print("%sERROR:\n"
                  "Freeing memory failed%s\n",
                  RED, CLEAR
            );
            break;
        }
        case FREE_PAGE_ALREADY_FREED: {
            print("%sERROR:\n"
                  "Freeing memory pages are already freed%s\n",
                  RED, CLEAR
            );
            my_exit("TRYING TO RELEASE AN ALREADY RELEASED MEM PAGE", __FILE__, __LINE__);

            break;
        }
    }
}
