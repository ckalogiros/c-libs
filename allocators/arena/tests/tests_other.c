/*
 * Created by ckalo on 3/14/26.
 */

#include "tests_other.h"
#include <assert.h>
#include <string.h>
#include "alignment.h"
#include "logger.h"
#include "my_macros.h"
#include "arena_allocator.h"
#include "arena_allocator_fprint.h"


void
test_arena_allocator(void)
{
    const arena_table_t inst_id = arena_allocator_create_new_instance(4);

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
        void* address = arena_allocator_alloc(inst_id, size + 1, sizeof(some_text[0][0]));
        memcpy(address, some_text[i], size);
        *((char*) address + size) = '\0';

        print("[%u] %s\n", i, address);
    }

    /* test allocator: release */
    void* address = arena_allocator_release(inst_id);
    print("FREEING ALLOCATOR: start address: %p\n", address);

}

#define test_arena_alignment(ptr, typesize){\
    uint32 alignment = align_pointer_to_type(ptr, typesize);\
    assert(alignment == 0);\
    print_test_result("alignement pass", TRUE, __FILE__, __LINE__, "");\
}

#define test_arena_equality_value(ptr, val, val1, type) {\
    val = *(type*)ptr;\
    print("val:%u == val1:%u\n", val, val1);\
    assert(val1 == val);\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_arena_alignment(ptr, sizeof(val1));\
}

#define test_arena_equality_string(ptr, buf, type) {\
    const type* str = (type*)ptr;\
    print("str:%s == buf:%s\n", str, buf);\
    assert(!strcmp(str, buf));\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_arena_alignment(ptr, sizeof(char));\
}

#define test_arena_equality_integer_buffer(ptr, buf, type, size) {\
    const type* buf0 = (type*)ptr;\
    print("buf:%d == buf:%d\n", (type)buf0[0], buf[0]);\
    for(uint32 i = 0; i < size; i++) {\
        assert((type)buf0[i] == buf[i]);\
    }\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_arena_alignment(ptr, sizeof(type));\
}
#define test_arena_equality_buffer(ptr, buf, type, size) {\
    const type* buf0 = (type*)ptr;\
    print("buf:%s == buf:%s\n", buf0, buf);\
    for(uint32 i = 0; i < size; i++) {\
        assert(buf0[i] == buf[i]);\
    }\
    print_test_result("equality pass", TRUE, __FILE__, __LINE__, "");\
    test_arena_alignment(ptr, sizeof(type));\
}


void
test_arena_allocator_alignment(void)
{
    void* address = NULL;
    char* addr = NULL;
    const arena_table_t inst_id = arena_allocator_create_new_instance(4);
    char* start_address = debug_arena_allocator_instance_get_data(inst_id);
    uint32 total_size = 0;

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

        addr = arena_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(addr, &val1, sizeof(val1));
        addr = arena_allocator_alloc(inst_id, sizeof(val2), sizeof(val2)); memcpy(addr, &val2, sizeof(val2));
        addr = arena_allocator_alloc(inst_id, sizeof(val3), sizeof(val3)); memcpy(addr, &val3, sizeof(val3));
        addr = arena_allocator_alloc(inst_id, sizeof(val4), sizeof(val4)); memcpy(addr, &val4, sizeof(val4));

        /* test allocator: */
        uint64 val = 0;

        uint32 prev_size = 0;
        uint32 unaligned = 0;

        address = start_address;

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val1));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val2));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_value(address, val, val2, uint16); prev_size = sizeof(val2);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val3));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_value(address, val, val3, typeof(val3)); prev_size = sizeof(val3);

        address += prev_size; /// Add the previous value's size to the pointer
        unaligned = align_pointer_to_type(address, sizeof(val4)); /// if ptr is unaligned to the current value's type: align the pointer
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_value(address, val, val4, uint64); prev_size = sizeof(val4);


        /* print memory */
        print_mem_hex(total_size, start_address);
        struct arena_allocator_instance* arena = debug_arena_allocator_get_instance(inst_id);
        fprint_arena_mem_data(arena);

        arena_allocator_release(inst_id);
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
        addr = arena_allocator_alloc(inst_id, sizeof(val4), sizeof(val4)); memcpy(addr, &val4, sizeof(val4));
        addr = arena_allocator_alloc(inst_id, sizeof(val3), sizeof(val3)); memcpy(addr, &val3, sizeof(val3));
        addr = arena_allocator_alloc(inst_id, sizeof(val2), sizeof(val2)); memcpy(addr, &val2, sizeof(val2));
        addr = arena_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(addr, &val1, sizeof(val1));
        addr = arena_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(addr, &val1, sizeof(val1));
        addr = arena_allocator_alloc(inst_id, sizeof(val2), sizeof(val2)); memcpy(addr, &val2, sizeof(val2));
        addr = arena_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(addr, &val1, sizeof(val1));
        addr = arena_allocator_alloc(inst_id, sizeof(val3), sizeof(val3)); memcpy(addr, &val3, sizeof(val3));
        addr = arena_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(addr, &val1, sizeof(val1));
        addr = arena_allocator_alloc(inst_id, sizeof(val4), sizeof(val4)); memcpy(addr, &val4, sizeof(val4));

        /* test allocator: alignment: */
        uint64 val = 0;
        uint32 prev_size = 0;
        uint32 unaligned = 0;

        {
            address = start_address;

            address += prev_size; /// Add the previous value's size to the pointer
            unaligned = align_pointer_to_type(address, sizeof(val4)); /// if ptr is unaligned to the current value's type: align the pointer
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_arena_equality_value(address, val, val4, uint64); prev_size = sizeof(val4);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val3));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_arena_equality_value(address, val, val3, uint32); prev_size = sizeof(val3);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val2));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_arena_equality_value(address, val, val2, uint16); prev_size = sizeof(val2);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val1));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_arena_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val1));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_arena_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val2));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_arena_equality_value(address, val, val2, uint16); prev_size = sizeof(val2);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val1));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_arena_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val3));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_arena_equality_value(address, val, val3, uint32); prev_size = sizeof(val3);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val1));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_arena_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

            address += prev_size;
            unaligned = align_pointer_to_type(address, sizeof(val4));
            if(unaligned) {address += unaligned; unaligned = 0; }
            test_arena_equality_value(address, val, val4, uint64); prev_size = sizeof(val4);
        }


        /* print memory */
        print_mem_hex(total_size, start_address);
        struct arena_allocator_instance* arena = debug_arena_allocator_get_instance(inst_id);
        fprint_arena_mem_data(arena);

        arena_allocator_release(inst_id);
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

        address = arena_allocator_alloc(inst_id, sizeof(val1), sizeof(val1)); memcpy(address, &val1, sizeof(val1));
        address = arena_allocator_alloc(inst_id, sizeof(val3), sizeof(val3)); memcpy(address, &val3, sizeof(val3));
        address = arena_allocator_alloc(inst_id, sizeof(val2), sizeof(val2)); memcpy(address, &val2, sizeof(val2));
        address = arena_allocator_alloc(inst_id, sizeof(buf16), sizeof(char)); memcpy(address, &buf16, sizeof(buf16));
        address = arena_allocator_alloc(inst_id, sizeof(val4), sizeof(val4)); memcpy(address, &val4, sizeof(val4));


        /* test allocator: alignment: */
        uint64 val = 0;
        uint32 prev_size = 0;
        uint32 unaligned = 0;
        address = start_address;

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val1));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_value(address, val, val1, uint8); prev_size = sizeof(val1);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val3));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_value(address, val, val3, uint32); prev_size = sizeof(val3);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val2));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_value(address, val, val2, uint16); prev_size = sizeof(val2);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf16[0]));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_value(address, val, buf16[0], char);
        prev_size = sizeof(buf16);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(val4));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_value(address, val, val4, uint64); prev_size = sizeof(val4);



        /* print memory */
        print_mem_hex(total_size, start_address);
        struct arena_allocator_instance* arena = debug_arena_allocator_get_instance(inst_id);
        fprint_arena_mem_data(arena);

        address = arena_allocator_release(inst_id);
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


        address = arena_allocator_alloc(inst_id, sizeof(buf2), sizeof(char)); memcpy(address, buf2, sizeof(buf2));
        address = arena_allocator_alloc(inst_id, sizeof(buf4), sizeof(char)); memcpy(address, buf4, sizeof(buf4));
        address = arena_allocator_alloc(inst_id, sizeof(buf3), sizeof(char)); memcpy(address, buf3, sizeof(buf3));
        address = arena_allocator_alloc(inst_id, sizeof(buf1), sizeof(char)); memcpy(address, buf1, sizeof(buf1));

        /* test allocator: alignment: */
        uint32 prev_size = 0;
        uint32 unaligned = 0;
        address = start_address;

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf2[0]));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_buffer(address, buf2, uint8, sizeof(buf2)); prev_size = sizeof(buf2);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf4[0]));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_buffer(address, buf4, uint8, sizeof(buf4)); prev_size = sizeof(buf4);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf3[0]));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_buffer(address, buf3, uint8, sizeof(buf3)); prev_size = sizeof(buf3);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf1[0]));
        if(unaligned) {address += unaligned; unaligned = 0; }
        test_arena_equality_buffer(address, buf1, uint8, sizeof(buf1)); prev_size = sizeof(buf1);


        /* print memory */
        print_mem_hex(total_size, start_address);
        struct arena_allocator_instance* arena = debug_arena_allocator_get_instance(inst_id);
        fprint_arena_mem_data(arena);

        address = arena_allocator_release(inst_id);
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


        address = arena_allocator_alloc(inst_id, sizeof(buf1), sizeof(buf1[0]));
        memcpy(address, buf1, sizeof(buf1));
        address = arena_allocator_alloc(inst_id, sizeof(buf2), sizeof(buf2[0]));
        memcpy(address, buf2, sizeof(buf2));
        address = arena_allocator_alloc(inst_id, sizeof(buf3), sizeof(buf3[0]));
        memcpy(address, buf3, sizeof(buf3));
        address = arena_allocator_alloc(inst_id, sizeof(buf4), sizeof(buf4[0]));
        memcpy(address, buf4, sizeof(buf4));
        /* again, insert the first 2 buffers */
        // address = arena_allocator_alloc(inst_id, sizeof(buf1), sizeof(uint8 )); memcpy(address, buf1, sizeof(buf1));
        // address = arena_allocator_alloc(inst_id, sizeof(buf2), sizeof(uint32)); memcpy(address, buf2, sizeof(buf2));


        /* test allocator: alignment: */
        uint32 prev_size = 0;
        uint32 unaligned = 0;
        address = start_address;

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf1[0]));
        if(unaligned) { address += unaligned; unaligned = 0; }
        test_arena_equality_integer_buffer(address, buf1, b1, sizeof(buf1)); prev_size = sizeof(buf1);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf2[0]));
        if(unaligned) { address += unaligned; unaligned = 0; }
        test_arena_equality_integer_buffer(address, buf2, b2, sizeof(buf2)/sizeof(b2)); prev_size = sizeof(buf2);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf3[0]));
        if(unaligned) { address += unaligned; unaligned = 0; }
        test_arena_equality_integer_buffer(address, buf3, b3, sizeof(buf3)/sizeof(b3)); prev_size = sizeof(buf3);

        address += prev_size;
        unaligned = align_pointer_to_type(address, sizeof(buf4[0]));
        if(unaligned) { address += unaligned; unaligned = 0; }
        test_arena_equality_integer_buffer(address, buf4, b4, sizeof(buf4)/sizeof(b4)); prev_size = sizeof(buf4);


        /* print memory */
        print_mem_hex(total_size, start_address);
        struct arena_allocator_instance* arena = debug_arena_allocator_get_instance(inst_id);
        fprint_arena_mem_data(arena);

        address = arena_allocator_release(inst_id);
    }


    
}
