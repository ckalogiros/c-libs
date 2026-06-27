/*
 * Created by ckalo on 3/15/26.
 */
#ifndef G_INCLUDES_ALIGNMENT_H
#define G_INCLUDES_ALIGNMENT_H
#include "my_macros.h"
#include "my_types.h"

static inline uint64
align_pointer_to_type(const void* ptr, const typesize_t type_size)
{
    assert(ptr);

    const char* aligned = (char*)ALIGN_SIZE(((uint64)ptr), type_size);
    if(aligned !=  ptr) { /* set(align) prev size to current type_size in the offsets table */
        const uint64 unaligned = (uint64)(aligned - (uint64)ptr);
        return unaligned;
    }

    return 0;
}

#endif //G_INCLUDES_ALIGNMENT_H