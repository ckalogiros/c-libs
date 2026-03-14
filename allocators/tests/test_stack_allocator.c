/*
 * Created by ckalo on 3/14/26.
 */

#include "test_stack_allocator.h"
#include <string.h>
#include "logger.h"
#include "mem_man_interface.h"
#include "stack_allocator.h"

void
test_stack_allocator(void)
{
    memman_init();

    const memid_t instance_id = stack_allocator_create_new_instance(4);

    const char* some_text[] = {
        "The devices, queues, and other entities in Vulkan are represented by Vulkan objects.",
        "At the API level, all objects are referred to by handles.",
        "There are two classes of handles: dispatchable and non-dispatchable",
        "Dispatchable handle types are a pointer to an opaque type.",
        "This pointer may be used by layers as part of intercepting API commands,",
        "and thus each API command takes a dispatchable type as its first parameter.",
        "Each object of a dispatchable type must have a unique handle value during its lifetime",
    };

    /* test allocator: push */
    for(uint32 i = 0; i < 7; i++) {
        const uint32 size = strlen(some_text[i]);
        void* address = stack_allocator_alloc(instance_id, size + 1);
        memcpy(address, some_text[i], size);
        *((char*) address + size) = '\0';

        // print("[%u] %s\n", i, address);
    }

    /* test allocator: pop */
    for(uint32 i = 0; i < 7; i++) {
        void* address = stack_allocator_release(instance_id);
        print("[%u] %s\n", i, address);
    }

    memman_terminate();
}
