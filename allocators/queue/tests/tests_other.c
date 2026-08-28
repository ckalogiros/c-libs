/*
 * Created by ckalo on 17/6/26.
 */

#include "tests_other.h"

#include <assert.h>
#include <stdalign.h>
#include <string.h>

#include "alignment.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "my_macros.h"
#include "queue_allocator.h"
#include "queue_allocator_fprint.h"
#include "str_utils.h"

void test_queue_allocator(void)
{
    const queue_table_t inst_id = queue_allocator_create_new_instance(1);

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
        void* address = queue_allocator_alloc(inst_id, size + 1, sizeof(some_text[0][0]));
        memcpy(address, some_text[i], size);
        *((char*) address + size) = '\0';

        print("[%u] %s\n", i, address);
    }

    /* test allocator: release */
    void* address = queue_allocator_reset(inst_id);
    print("FREEING ALLOCATOR: start address: %p\n", address);
    
}


/// TODO: copy this function to stack allocator too
void
test_queue_allocator_structs_arbitrary_sizes(void)
{
    const memid_t instance_id = queue_allocator_create_new_instance(1);
    void* mem_start_cache = NULL;

    struct A{int x; int y; float f;};
    struct B{int arr[12]; int x; void* ptr;};
    // struct B{int arr[12]; int x;};
    struct C{void* ptr; float arr[8];};
    struct AB{struct A a; struct B b;};
    struct AC{struct A a; struct C c;};
    struct BC{struct B b; struct C c;};

    struct A a = {11, 22, 33.3};
    struct B b = {{1,2,3,4,5,6,7,8,9,10,11,12}, 22, (void*)0xCDABF0};
    // struct B b = {{1,2,3,4,5,6,7,8,9,10,11,12}, 22};
    struct C c = {(void*)0xFFEEDDCC, {.1, .2, .3, .4, .5, .6, .7, .8}};
    struct AB ab = {a, b};
    struct AC ac = {a, c};
    struct BC bc = {b, c};

    /* test allocator: push */
    uint32 size = sizeof(struct A);
    uint32 color_index = 0;
    // void* address = queue_allocator_alloc(instance_id, sizeof(struct A), sizeof(struct A), 0xA);
    void* address = queue_allocator_alloc(instance_id, size, alignof(struct A));
    mem_start_cache = address;
    memcpy(address, &a, size);
    struct A* ptra = (struct A*)address;
    print("-- STRUCT A -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);
    print("  x:%d, x:%d, x:%.2f\n", ptra->x, ptra->y, ptra->f);

    size = sizeof(struct B);
    address = queue_allocator_alloc(instance_id, size, alignof(struct B));
    memcpy(address, &b, size);
    struct B* ptrb = (struct B*)address;
    print("-- STRUCT B -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);
    for(uint32 i = 0; i < ARRAY_LENGTH(b.arr); i++)
        print("  arr:%d, ", ptrb->arr[i]);
    print("\n");
    print("  x:%d, ptr:%p\n", ptrb->x, ptrb->ptr);

    size = sizeof(struct C);
    address = queue_allocator_alloc(instance_id, size, alignof(struct C));
    memcpy(address, &c, size);
    struct C* ptrc = (struct C*)address;
    print("-- STRUCT C -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);
    print("  ptr:%p\n", ptrc->ptr);
    for(uint32 i = 0; i < ARRAY_LENGTH(c.arr); i++)
        print("  arr:%.1f, ", ptrc->arr[i]);
    print("\n");

    size = sizeof(struct AB);
    address = queue_allocator_alloc(instance_id, size, alignof(struct AB));
    memcpy(address, &ab, size);
    struct AB* ptrab = (struct AB*)address;
    print("-- STRUCT AB -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);
    print("  x:%d, x:%d, x:%.2f\n", ptrab->a.x, ptrab->a.y, ptrab->a.f);
    for(uint32 i = 0; i < ARRAY_LENGTH(b.arr); i++)
        print("  arr:%d, ", ptrab->b.arr[i]);
    print("\n");

    size = sizeof(struct AC);
    address = queue_allocator_alloc(instance_id, size, alignof(struct AC));
    memcpy(address, &ac, size);
    struct AC* ptrac = (struct AC*)address;
    print("-- STRUCT AC -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);

    size = sizeof(struct BC);
    address = queue_allocator_alloc(instance_id, size, alignof(struct BC));
    memcpy(address, &bc, size);
    struct BC* ptrbc = (struct BC*)address;
    print("-- STRUCT BC -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);

    struct queue_allocator_instance* queue = debug_queue_allocator_get_instance(instance_id);
    fprint_queue_mem_data(queue);


    /* test allocator: pop/dequeue */
    uint32 total_alloc_size = 0;
    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_reset(instance_id);
    memset(address, 0xA, size); /// mark memory

    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_reset(instance_id);
    memset(address, 0xB, size); /// mark memory

    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_reset(instance_id);
    memset(address, 0xC, size); /// mark memory

    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_reset(instance_id);
    memset(address, 0xD, size); /// mark memory

    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_reset(instance_id);
    memset(address, 0xE, size); /// mark memory

    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_reset(instance_id);
    memset(address, 0xF, size); /// mark memory

    fprint_queue_mem_data(queue);


    /* print-debug memory */
    print("\n-- COMPARE THE SIZE OF EACH STRUCT WITH THE SIZE OF THE MEMORY FOOT PRINT --\n"
          "AND TAKE INTO CONSIDERATION ANY PADDING\n");
    color_index = 0;
    char* color = COLOR_ARRAY_GET(color_index);
    char* mem_ptr = (char*)mem_start_cache;
    uint32 cur_elem = 0xA;
    for(uint32 i = 0; i < total_alloc_size; i++) {

        if(cur_elem != mem_ptr[i]) {
            cur_elem = mem_ptr[i];
            color_index++;
            if(color_index > COLOR_ARRAY_COUNT) color_index = 0;
            color = COLOR_ARRAY_GET(color_index);
        }
        if((i%4)==0) print(" ");
        if((i%32)==0) print("\n");
        print("%s%2X", color, mem_ptr[i]);

    }


}


#define test_queue_alignment(ptr, typesize){\
    uint32 alignment = align_pointer_to_type(ptr, typesize);\
    assert(alignment == 0);\
    print_test_result("alignement pass", TRUE, __FILE__, __LINE__, "");\
}

#define test_queue_equality_value(ptr, val, val1, type) {\
    val = *(type*)ptr;\
    print("val:%u == val1:%u\n", val, val1);\
    assert(val1 == val);\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_queue_alignment(address, sizeof(val1));\
}

#define test_queue_equality_string(ptr, buf, type) {\
    const type* str = (type*)ptr;\
    print("str:%s == buf:%s\n", str, buf);\
    assert(!strcmp(str, buf));\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_queue_alignment(ptr, sizeof(char));\
}

#define test_queue_equality_buffer(ptr, buf, type) {\
    const type* buf = (type*)ptr;\
    print("buf:%s == buf:%s\n", buf, buf2);\
    /*assert(!buf32_cmp(buf, buf2, sizeof(buf2), sizeof(buf2)));*/\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_queue_alignment(ptr, sizeof(type));\
}

#define test_queue_alignment(ptr, typesize){\
    uint32 alignment = align_pointer_to_type(ptr, typesize);\
    assert(alignment == 0);\
    print_test_result("alignement pass", TRUE, __FILE__, __LINE__, "");\
}

#define test_queue_equality_value(ptr, val, val1, type) {\
    val = *(type*)ptr;\
    print("val:%u == val1:%u\n", val, val1);\
    assert(val1 == val);\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_queue_alignment(ptr, sizeof(val1));\
}

#define test_queue_equality_string(ptr, buf, type) {\
    const type* str = (type*)ptr;\
    print("str:%s == buf:%s\n", str, buf);\
    assert(!strcmp(str, buf));\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_queue_alignment(ptr, sizeof(char));\
}

#define test_queue_equality_integer_buffer(ptr, buf, type, size) {\
    const type* buf0 = (type*)ptr;\
    print("buf:%d == buf:%d\n", (type)buf0[0], buf[0]);\
    for(uint32 i = 0; i < size; i++) {\
        assert((type)buf0[i] == buf[i]);\
    }\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_queue_alignment(ptr, sizeof(type));\
}

#define test_queue_equality_buffer(ptr, buf, type, size) {\
    const type* buf0 = (type*)ptr;\
    print("buf:%s == buf:%s\n", buf0, buf);\
    for(uint32 i = 0; i < size; i++) {\
        assert(buf0[i] == buf[i]);\
    }\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_queue_alignment(ptr, sizeof(type));\
}


void
test_queue_allocator_alignment(void)
{
    void* address = NULL;
    char* addr = NULL;
    const queue_table_t inst_id = queue_allocator_create_new_instance(4);
    char* start_address = debug_queue_allocator_instance_get_data(inst_id);
    uint32 total_size = 0;
    struct queue_allocator_instance* queue = debug_queue_allocator_get_instance(inst_id);


    {
        total_size = 0;
        uint8  val1 = HACK_LITERAL_STR_TO_NUM("1", uint8);
        uint16 val2 = HACK_LITERAL_STR_TO_NUM("22", uint16);
        uint32 val3 = HACK_LITERAL_STR_TO_NUM("4444", uint32);
        uint64 val4 = HACK_LITERAL_STR_TO_NUM("88888888", uint64);

        total_size += sizeof(val1);
        total_size += sizeof(val2);
        total_size += sizeof(val3);
        total_size += sizeof(val4);

        memset(start_address, 0, total_size);

        print("%s-- TEST  integer types mixed --%s\n   total size:%u\n", ry, CLEAR, total_size);

        addr = queue_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(addr, &val1, sizeof(val1));
        addr = queue_allocator_alloc(inst_id, sizeof(val2), sizeof(val2)); memcpy(addr, &val2, sizeof(val2));
        addr = queue_allocator_alloc(inst_id, sizeof(val3), sizeof(val3)); memcpy(addr, &val3, sizeof(val3));
        addr = queue_allocator_alloc(inst_id, sizeof(val4), sizeof(val4)); memcpy(addr, &val4, sizeof(val4));

        /* test allocator: */
        uint64 val = 0;

        uint32 prev_size = 0;
        uint32 unaligned = 0;

        address = start_address;

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val1));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val2));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_value(address, val, val2, uint16); prev_size = sizeof(val2);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val3));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_value(address, val, val3, uint32); prev_size = sizeof(val3);

        address += prev_size; /// Add the previous value's size to the pointer
        unaligned = align_pointer_to_type(address, sizeof(val4)); /// if ptr is unaligned to the current value's type: align the pointer
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_value(address, val, val4, uint64); prev_size = sizeof(val4);


        /* print memory */
        print_mem_hex(total_size, start_address);
        fprint_queue_mem_data(queue);

        // address = queue_allocator_reset(inst_id);
        /* test allocator: pop-dequeue-release */
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
    }

    /***********************************************************************************************
     * REPEAT MIXED
     **********************************************************************************************/
    {
        total_size = 0;
        uint8  val1 = HACK_LITERAL_STR_TO_NUM("1", uint8);
        uint16 val2 = HACK_LITERAL_STR_TO_NUM("22", uint16);
        uint32 val3 = HACK_LITERAL_STR_TO_NUM("4444", uint32);
        uint64 val4 = HACK_LITERAL_STR_TO_NUM("88888888", uint64);

        total_size += sizeof(val1);
        total_size += sizeof(val2);
        total_size += sizeof(val3);
        total_size += sizeof(val4);

        memset(start_address, 0, total_size);

        print("%s-- TEST  integer types mixed --%s\n total size:%u\n", ry, CLEAR,  total_size);


        /* test allocator: push  */
        addr = queue_allocator_alloc(inst_id, sizeof(val4), sizeof(val4)); memcpy(addr, &val4, sizeof(val4));
        addr = queue_allocator_alloc(inst_id, sizeof(val3), sizeof(val3)); memcpy(addr, &val3, sizeof(val3));
        addr = queue_allocator_alloc(inst_id, sizeof(val2), sizeof(val2)); memcpy(addr, &val2, sizeof(val2));
        addr = queue_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(addr, &val1, sizeof(val1));
        addr = queue_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(addr, &val1, sizeof(val1));
        addr = queue_allocator_alloc(inst_id, sizeof(val2), sizeof(val2)); memcpy(addr, &val2, sizeof(val2));
        addr = queue_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(addr, &val1, sizeof(val1));
        addr = queue_allocator_alloc(inst_id, sizeof(val3), sizeof(val3)); memcpy(addr, &val3, sizeof(val3));
        addr = queue_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(addr, &val1, sizeof(val1));
        addr = queue_allocator_alloc(inst_id, sizeof(val4), sizeof(val4)); memcpy(addr, &val4, sizeof(val4));

        /* test allocator: alignment: */
        uint64 val = 0;
        uint32 prev_size = 0;
        uint32 unaligned = 0;
        address = start_address;

        {
            address += prev_size; /// Add the previous value's size to the pointer
            unaligned = align_pointer_to_type(address, sizeof(val4)); /// if ptr is unaligned to the current value's type: align the pointer
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_queue_equality_value(address, val, val4, uint64); prev_size = sizeof(val4);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val3));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_queue_equality_value(address, val, val3, uint32); prev_size = sizeof(val3);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val2));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_queue_equality_value(address, val, val2, uint16); prev_size = sizeof(val2);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val1));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_queue_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val1));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_queue_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val2));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_queue_equality_value(address, val, val2, uint16); prev_size = sizeof(val2);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val1));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_queue_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val3));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_queue_equality_value(address, val, val3, uint32); prev_size = sizeof(val3);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val1));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_queue_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val4));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_queue_equality_value(address, val, val4, uint64); prev_size = sizeof(val4);
        }


        /* print memory */
        print_mem_hex(total_size, start_address);
        fprint_queue_mem_data(queue);

        // address = queue_allocator_reset(inst_id);
        /* test allocator: pop-dequeue-release */
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
    }



    /***********************************************************************************************
     * REPEAT MIXED
     **********************************************************************************************/
    {
        total_size = 0;
        uint8  val1 = HACK_LITERAL_STR_TO_NUM("1", uint8);
        uint16 val2 = HACK_LITERAL_STR_TO_NUM("22", uint16);
        uint32 val3 = HACK_LITERAL_STR_TO_NUM("4444", uint32);
        uint64 val4 = HACK_LITERAL_STR_TO_NUM("88888888", uint64);
        char buf16[11] = "123456789a\0";

        total_size += sizeof(val1);
        total_size += sizeof(val2);
        total_size += sizeof(val3);
        total_size += sizeof(val4);
        total_size += sizeof(buf16);

        memset(start_address, 0, total_size);

        print("%s-- TEST integer types mixed with char buf[] --%s\n   total size:%u\n", ry, CLEAR,  total_size);

        address = queue_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(address, &val1, sizeof(val1));
        address = queue_allocator_alloc(inst_id, sizeof(val3), sizeof(val3)); memcpy(address, &val3, sizeof(val3));
        address = queue_allocator_alloc(inst_id, sizeof(val2), sizeof(val2)); memcpy(address, &val2, sizeof(val2));
        address = queue_allocator_alloc(inst_id, sizeof(buf16), sizeof(char)); memcpy(address, &buf16, sizeof(buf16));
        address = queue_allocator_alloc(inst_id, sizeof(val4), sizeof(val4)); memcpy(address, &val4, sizeof(val4));


        /* test allocator: alignment */
        uint64 val = 0;
        uint32 prev_size = 0;
        uint32 unaligned = 0;
        address = start_address;

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val1));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val3));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_value(address, val, val3, uint32); prev_size = sizeof(val3);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val2));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_value(address, val, val2, uint16); prev_size = sizeof(val2);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf16[0]));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_value(address, val, buf16[0], char);
        prev_size = sizeof(buf16);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val4));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_value(address, val, val4, uint64); prev_size = sizeof(val4);



        /* print memory */
        print_mem_hex(total_size, start_address);
        fprint_queue_mem_data(queue);

        // address = queue_allocator_reset(inst_id);
        /* test allocator: pop-dequeue-release */
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
    }


    /***********************************************************************************************
     * MIXED: char buffers
     **********************************************************************************************/
    {
        total_size = 0;
        const uint8 buf1[4] = {0};
        const uint8 buf2[12] = {0};
        const uint8 buf3[3] = {0};
        const uint8 buf4[12] = {0};
        memcpy(buf1, "aaa\0", sizeof(buf1));
        memcpy(buf2, "sssssssssss\0", sizeof(buf2));
        memcpy(buf3, "cc\0", sizeof(buf3));
        memcpy(buf4, "ddddddddddd\0", sizeof(buf4));

        total_size += sizeof(buf1);
        total_size += sizeof(buf2);
        total_size += sizeof(buf3);
        total_size += sizeof(buf4);

        memset(start_address, 0, total_size);

        print("%s-- TEST char buffers[] --%s\n   total size:%u\n", ry, CLEAR,  total_size);


        address = queue_allocator_alloc(inst_id, sizeof(buf2), sizeof(char)); memcpy(address, buf2, sizeof(buf2));
        address = queue_allocator_alloc(inst_id, sizeof(buf4), sizeof(char)); memcpy(address, buf4, sizeof(buf4));
        address = queue_allocator_alloc(inst_id, sizeof(buf3), sizeof(char)); memcpy(address, buf3, sizeof(buf3));
        address = queue_allocator_alloc(inst_id, sizeof(buf1), sizeof(char)); memcpy(address, buf1, sizeof(buf1));

        /* test allocator: alignment: */
        uint32 prev_size = 0;
        uint32 unaligned = 0;
        address = start_address;

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf2[0]));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_buffer(address, buf2, uint8, sizeof(buf2)); prev_size = sizeof(buf2);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf4[0]));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_buffer(address, buf4, uint8, sizeof(buf4)); prev_size = sizeof(buf4);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf3[0]));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_buffer(address, buf3, uint8, sizeof(buf3)); prev_size = sizeof(buf3);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf1[0]));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_queue_equality_buffer(address, buf1, uint8, sizeof(buf1)); prev_size = sizeof(buf1);


        /* print memory */
        print_mem_hex(total_size, start_address);
        fprint_queue_mem_data(queue);

        // address = queue_allocator_reset(inst_id);
        /* test allocator: pop-dequeue-release */
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
    }


    /***********************************************************************************************
     * MIXED: uint32 buffers
     **********************************************************************************************/
    {
        total_size = 0;
        typedef uint8  b1;
        typedef uint32 b2;
        typedef uint8  b3;
        typedef uint64 b4;
        b1 buf1[6] = {0};
        b2 buf2[2] = {0};
        b3 buf3[2] = {0};
        b4 buf4[1] = {0};

        char* ptr = (char*)buf1;
        uint32 i = 0;
        for(; i < sizeof(buf1); i++) { ptr[i] = 'a'; } /*ptr[i] = '\0';*/ i = 0; ptr = (char*)buf2;
        for(; i < sizeof(buf2); i++) { ptr[i] = 'b'; } /*ptr[i] = '\0';*/ i = 0; ptr = (char*)buf3;
        for(; i < sizeof(buf3); i++) { ptr[i] = 'c'; } /*ptr[i] = '\0';*/ i = 0; ptr = (char*)buf4;
        for(; i < sizeof(buf4); i++) { ptr[i] = 'd'; } /*ptr[i] = '\0';*/ i = 0;

        total_size += sizeof(buf1);
        total_size += sizeof(buf2);
        total_size += sizeof(buf3);
        total_size += sizeof(buf4);

        memset(start_address, 0, total_size);

        print("%s-- TEST uint32 buffers --%s\n   total size:%u\n", ry, CLEAR,  total_size);


        address = queue_allocator_alloc(inst_id, sizeof(buf1), sizeof(buf1[0])); memcpy(address, buf1, sizeof(buf1));
        address = queue_allocator_alloc(inst_id, sizeof(buf2), sizeof(buf2[0])); memcpy(address, buf2, sizeof(buf2));
        address = queue_allocator_alloc(inst_id, sizeof(buf3), sizeof(buf3[0])); memcpy(address, buf3, sizeof(buf3));
        address = queue_allocator_alloc(inst_id, sizeof(buf4), sizeof(buf4[0])); memcpy(address, buf4, sizeof(buf4));
        /* again, insert the first 2 buffers */
        address = queue_allocator_alloc(inst_id, sizeof(buf1), sizeof(uint8 )); memcpy(address, buf1, sizeof(buf1));
        address = queue_allocator_alloc(inst_id, sizeof(buf2), sizeof(uint32)); memcpy(address, buf2, sizeof(buf2));


        /* test allocator: alignment: */
        uint32 prev_size = 0;
        uint32 unaligned = 0;
        address = start_address;

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf1[0]));
        if(unaligned) { address += unaligned; unaligned = 0; }
        test_queue_equality_integer_buffer(address, buf1, b1, sizeof(buf1)); prev_size = sizeof(buf1);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf2[0]));
        if(unaligned) { address += unaligned; unaligned = 0; }
        test_queue_equality_integer_buffer(address, buf2, b2, sizeof(buf2)/sizeof(b2)); prev_size = sizeof(buf2);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf3[0]));
        if(unaligned) { address += unaligned; unaligned = 0; }
        test_queue_equality_integer_buffer(address, buf3, b3, sizeof(buf3)/sizeof(b3)); prev_size = sizeof(buf3);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf4[0]));
        if(unaligned) { address += unaligned; unaligned = 0; }
        test_queue_equality_integer_buffer(address, buf4, b4, sizeof(buf4)/sizeof(b4)); prev_size = sizeof(buf4);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf1[0]));
        if(unaligned) { address += unaligned; unaligned = 0; }
        test_queue_equality_integer_buffer(address, buf1, b1, sizeof(buf1)); prev_size = sizeof(buf1);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf2[0]));
        if(unaligned) { address += unaligned; unaligned = 0; }
        test_queue_equality_integer_buffer(address, buf2, b2, sizeof(buf2)/sizeof(b2)); prev_size = sizeof(buf2);

        /* print memory */
        print_mem_hex(total_size, start_address);
        fprint_queue_mem_data(queue);

        // address = queue_allocator_reset(inst_id);
        /* test allocator: pop-dequeue-release */
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
        address = queue_allocator_dequeue(inst_id);
    }

}
