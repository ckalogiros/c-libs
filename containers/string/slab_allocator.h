/*
 * Created by ckalo on 3/12/26.
 *
 * should an allocator have its own memory for naming its structures?
 * dedicate a page of the allocation
 */
#ifndef STRING_SLAB_ALLOCATOR_H
#define STRING_SLAB_ALLOCATOR_H

#include "mem_structs.h"
#include "my_types.h"

// initialize memory for allocator instance: memory_ids_table
// memory_ids_buffer is extendable
// implememt destructor or release()
// implememt destructor or realloc()


// initialize memory for instance's: mcaches_table
// implememt destructor or release()
// implememt destructor or realloc()

/********************************************************/
typedef int8 salltable_t;
struct slab_allocator_instance {
    // void* instance;
    memid_t instance_id;
    salltable_t count;
    salltable_t size;
};

void slab_allocator_instance_create(){}






/********************************************************/
/* devide mcaches into zones: small, mid, large
 * each one have its seperate pool of memory */
struct slab_allocator_pool {
    nameid_t* name;
    void* start;
    uint64 size;
    pageid_t num_pages;
    memid_t mem_id;
};





/********************************************************/
typedef uint32 slab_size_t;
struct string_slab_flist { // TODO: flist struct already implemented
    struct string_slab_flist* next;
};

struct string_mem_cache {
    struct string_slab_flist* slabs_free_list;
    void* slabs_start; // the start of slabs alocated memory
    void* last_slab_end; // the start of slabs alocated memory
    slab_size_t slab_size; // each slab's size in bytes
    uint16 slabs_capacity; // total slabs count
    uint16 slabs_count; // used/unused slabs count
    uint16 slabs_used; // used slabs count
    uint16 slabs_num_pages; // number of pages of all the slabs
    struct memhnd_t mem_handle;
    uint8 idx; // index in the mcaches table
    uint8 is_active; // the stored objects type

    char_buf16_t name; // todo: later use indexed string buffer
};


#endif //STRING_SLAB_ALLOCATOR_H