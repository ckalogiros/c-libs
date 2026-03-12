/*
 * Created by ckalo on 3/12/26.
 */
#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include "mem_structs.h"
#include "my_types.h"

typedef int32 nameid_t;
typedef int8 balltable_t;
struct bump_allocator_instance {
    memid_t instance_id;
    uint32 bump_ptr;
    aralltable_t count;
    aralltable_t size;
};



#endif //ARENA_ALLOCATOR_H