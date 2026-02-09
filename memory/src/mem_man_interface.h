/*
 * Created by ckalo on 2/8/26.
 */
#ifndef MEMORY_MEM_MAN_TYPES_H
#define MEMORY_MEM_MAN_TYPES_H


#define NUM_CHUNKS 16


#define PAGE_SIZE 1024


/* *************************************
 * bitfield for chunk_id, page_id, etc */
typedef uint32 memids_t;

/* *********************************
 * max type of an id of a bitfield
 * (chunk_id, page_id, etc) */
typedef uint16 memid_t;





enum MEM_CHUNK_SELECTOR {
    MEM_CHUNK_ALLOCATOR_SLAB = 0,

    MEM_CHUNK_ANY = NUM_CHUNKS-1, // leave initial chunks for specific allocators
};

enum MEMORY_ERROR_CODE {
    NO_MEMORY_MANAGER_INIT = BIT_MASK_FROM_UINT(1),
    NO_FREE_MEMORY = BIT_MASK_FROM_UINT(2),
    MEMMAN_ALREADY_INITIALIZED = BIT_MASK_FROM_UINT(3),
    ALLOC_NOT_CONTIGUOUS_MEM = BIT_MASK_FROM_UINT(4),
    MEMORY_IS_IN_USE = BIT_MASK_FROM_UINT(5),
};


/* ************************************
 * allocation's return type to caller */
struct memhnd_t {
    memid_t chunk_id;
    memid_t page_id;
    memid_t num_pages;
};


#endif //MEMORY_MEM_MAN_TYPES_H