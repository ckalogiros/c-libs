/*
 * Created by ckalo on 1/1/26.
 */

#include "mem_log.h"
#include "mem_structs.h"
#include "mem_man_interface.h"
#include "logger.h"


#define DEBUG_PRINT_MEM
// #undef DEBUG_PRINT_MEM // undef to remove print info
#define DEBUG_HTML_DIR "/data/projects/clion/memory/memory_debug.html"



void logger_to_html_init()
{
    // printhtml_init(DEBUG_HTML_DIR);
}
void logger_to_html_destroy()
{
    // printhtml_close(DEBUG_HTML_DIR);
    // printhtml_destroy();
}

void
mem_log_print_init_mem()
{
#ifdef DEBUG_PRINT_MEM
    print("%s# INITIALIZE MEMORY #%s\n\n", YELLOW, CLEAR);
#endif
}

void
mem_log_print_terminate_mem()
{
#ifdef DEBUG_PRINT_MEM
    print("\n\n%s# TERMINATE MEMORY #%s\n", YELLOW, CLEAR);
#endif
}

void
mem_log_print_mem(const struct mem* const mem)
{
#ifdef DEBUG_PRINT_MEM
    print("-- %smemory%s --\n", ry, CLEAR);
    print(
        " size bytes:  %s%lu, %lukb%s\n"
        " data size:   %s%lu, %lukb%s\n"
        " num chunks:  %s%u%s\n"
        " total pages: %s%u%s\n"
        " page size:   %s%u%s\n"
        " mem start:      %s%x%s\n"
        " chunks start:   %s%x%s\n"
        " data start:     %s%x%s\n"
        " data last page: %s%x%s\n"
        " last page:      %s%x%s\n"
        " unauth page:    %s%x%s\n",
        YELLOW, mem->size, mem->size/PAGE_SIZE, CLEAR,
        YELLOW, mem->data_size, mem->data_size/PAGE_SIZE, CLEAR,
        YELLOW, mem->chunks_count, CLEAR,
        YELLOW, mem->chunks_count * PAGES_PER_CHUNK, CLEAR,
        YELLOW, PAGE_SIZE, CLEAR,
        CYAN, REDUCE_ADDR((char*)mem->start, F6), CLEAR,
        CYAN, REDUCE_ADDR((char*)MEM_GET_CHUNKS_TABLE_START, F6), CLEAR,
        CYAN, REDUCE_ADDR((char*)mem->start + mem->self_size, F6), CLEAR,
        CYAN, REDUCE_ADDR((char*)mem->start + mem->data_size, F6), CLEAR,
        CYAN, REDUCE_ADDR((char*)mem->start + mem->size - mem->bound_check_size, F6), CLEAR,
        CYAN, REDUCE_ADDR((char*)mem->start + mem->size, F6), CLEAR
    );


    print("\n");
#endif
}

void
mem_log_print_free_list_of_chunk(const memid_t chunk_id)
{
#ifdef DEBUG_PRINT_MEM
    print("-- %sfree list chunk id:%u%s --\n", ry, chunk_id, CLEAR);
    const struct free_list* flist = flist_table_get_head(chunk_id);
    uint32 k = 0;
    while(flist) {
        print(
            "[%s%u%s]"
            " addr:%s%x%s"
            " color:%s%s%s\n",
            // " id:%s%u%s\n",
            CYAN, k, CLEAR,
            CYAN, (uint64)(flist)&F6, CLEAR,
            CYAN, FREE_LIST_GET_PAGE_COLOR_START(flist), CLEAR
            // br2, (uint64)(flist->next)&F6, CLEAR
        );
        flist = flist->next;
        k++;
        if(k>1000) break; // avoid infinite loop
    }
    print("free pages:%s%u%s  bytesize:%s%u%s\n", YELLOW, k, CLEAR, YELLOW, k * PAGE_SIZE, CLEAR);
    // print("-- %sfree list%s --\n\n", ry, CLEAR);
#endif
}

void
mem_log_print_free_list_table()
{
#ifdef DEBUG_PRINT_MEM
    print("-- %sfree list heads table%s --\n", ry, CLEAR);

    struct free_list** ftable = debug_free_list_table_get();
    uint16 chunks_count = debug_mem_get_chunks_count();

    for(uint32 i = 0; i < chunks_count; i++) {
        print(
            "[%s%u%s]"
            " addr:%s%x%s\n",
            CYAN, i, CLEAR,
            CYAN, (uint64)(ftable[i])&F6, CLEAR
            // br2, (uint64)(flist->next)&F6, CLEAR
        );
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
      br2, flist_table_get_head(hnd.chunk_id), CLEAR
      );
}

void
mem_print_mem_release(void* released_addr, const struct memhnd_t hnd)
{
    print("%smem_release()%s"
          " page:%s%u%s"
          " addr:%s%x%s"
          " nextFree:%s%x%s\n",
          ry, CLEAR,
          YELLOW, hnd.page_id, CLEAR,
          br1, released_addr, CLEAR,
          br2, flist_table_get_head(hnd.chunk_id), CLEAR
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
                  "Call memman_init() and don't forget memory_manager_terminate()%s\n",
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
    }
}
