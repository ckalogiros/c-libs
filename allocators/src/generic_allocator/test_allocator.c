/*
 * Created by ckalo on 1/10/26.
 *
 * todo: explanation  - free_offset and the free list implementation
 */

#include "test_allocator.h"
#include "mem_man.h"
#include "mem_man_internals.h"
#include "mem_log.h"
#include <stdlib.h>


#define ALLOCATOR_BUFFER_SIZE 1024

typedef struct allocation_t {
    void* addr;
    uint32 idx;         // self reference index
} allocation_t;

typedef void* (*alloc_fn) ();
typedef void* (*free_fn) (void* addr);

struct allocator_t {
    alloc_fn alloc;
    free_fn free;

    uint16 free_list_offset; // offset (in bytes) to the start of the free elems list
    uint16 data_offset;      // offset (in bytes) to the start of the buffer's data
    allocation_t* data;      // buffer start (allocations of type allocations_t)
    uint16 capacity;         // total buffer  elements count
    uint16 count;            // used elements count
    int16  tail;             // last element's index
    uint16 idx;              // self reference index
};

static struct allocator_t* s_allocator;

void allocator_init()
{
    // Only one instance allowed
    if (s_allocator) return;

    // how many uint64's do we need to cover all allocator's buffer elements. 16 uint64's for buffer capacity=1024
    const uint32 free_elems_uint64_count = (ALLOCATOR_BUFFER_SIZE / BITS64);
    // size in bytes of the bitwise free_list
    const uint32 free_list_size_in_bytes = free_elems_uint64_count * sizeof(uint64);

        const uint32 total_memory_in_bytes   = sizeof(allocation_t) * ALLOCATOR_BUFFER_SIZE;
    const void* const addr            = aligned_alloc(1024, total_memory_in_bytes);

    struct allocator_t* allocator = (struct allocator_t*)addr;
    allocator->free_list_offset   = (uint64)sizeof(struct allocator_t);
    allocator->data_offset        = allocator->free_list_offset + free_list_size_in_bytes;
    allocator->data               = (allocation_t*) (addr + allocator->data_offset);
    allocator->capacity           = ALLOCATOR_BUFFER_SIZE;
    allocator->count              = 0;
    allocator->tail               = INT_NULL;
    allocator->idx                = INT_NULL;

    s_allocator = allocator; // one instance, global access to allocator source code
}
pageid_t allocator_test_alloc(const uint32 byte_size)
{
    void* mem_ptr = mem_get_address();
    if (!mem_ptr) {
        mem_print_error(NO_MEMORY_MANAGER_INIT);
    }

    // todo: check if no memory left
    // todo: create an apropriate allocators num pages allocation
    pageid_t num_pages = 0;
    if (byte_size < PAGE_SIZE) { num_pages = 1; }
    else if (byte_size < PAGE_SIZE*2) { num_pages = 2; }
    else if (byte_size < PAGE_SIZE*3) { num_pages = 3; }
    const pageid_t page_id = mmu_allocate_pages(num_pages);

    return page_id;
}
