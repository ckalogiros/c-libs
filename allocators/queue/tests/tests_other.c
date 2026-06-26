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
#include "str_utils.h"


void
test_queue_allocator_typesize_byte(void)
{
    const memid_t instance_id = queue_allocator_create_new_instance(4);

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
        void* address = queue_allocator_alloc(instance_id, size + 1, sizeof(some_text[0][0]), 0xCD);
        memcpy(address, some_text[i], size);
        *((char*) address + size) = '\0';

        // print("[%u] %s\n", i, address);
    }

    /* test allocator: pop */
    for(uint32 i = 0; i < 7; i++) {
        uint32 size = queue_allocator_get_last_elem_size(instance_id);
        void* address = queue_allocator_release(instance_id);

        print("[%u] %s\n", i, address);

        /* debug-mark released memory with value */
        memset(address, 0xCD, size);
    }

}

/// TODO: copy this function to stack allocator too
void
test_queue_allocator_typesize_arbitrary(void)
{
    const memid_t instance_id = queue_allocator_create_new_instance(4);
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
    void* address = queue_allocator_alloc(instance_id, size, alignof(struct A), 0xA);
    mem_start_cache = address;
    memcpy(address, &a, size);
    struct A* ptra = (struct A*)address;
    print("-- STRUCT A -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);
    print("  x:%d, x:%d, x:%.2f\n", ptra->x, ptra->y, ptra->f);

    size = sizeof(struct B);
    address = queue_allocator_alloc(instance_id, size, alignof(struct B), 0xB);
    memcpy(address, &b, size);
    struct B* ptrb = (struct B*)address;
    print("-- STRUCT B -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);
    for(uint32 i = 0; i < ARRAY_LENGTH(b.arr); i++)
        print("  arr:%d, ", ptrb->arr[i]);
    print("\n");
    print("  x:%d, ptr:%p\n", ptrb->x, ptrb->ptr);

    size = sizeof(struct C);
    address = queue_allocator_alloc(instance_id, size, alignof(struct C), 0xC);
    memcpy(address, &c, size);
    struct C* ptrc = (struct C*)address;
    print("-- STRUCT C -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);
    print("  ptr:%p\n", ptrc->ptr);
    for(uint32 i = 0; i < ARRAY_LENGTH(c.arr); i++)
        print("  arr:%.1f, ", ptrc->arr[i]);
    print("\n");

    size = sizeof(struct AB);
    address = queue_allocator_alloc(instance_id, size, alignof(struct AB), 0xAB);
    memcpy(address, &ab, size);
    struct AB* ptrab = (struct AB*)address;
    print("-- STRUCT AB -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);
    print("  x:%d, x:%d, x:%.2f\n", ptrab->a.x, ptrab->a.y, ptrab->a.f);
    for(uint32 i = 0; i < ARRAY_LENGTH(b.arr); i++)
        print("  arr:%d, ", ptrab->b.arr[i]);
    print("\n");

    size = sizeof(struct AC);
    address = queue_allocator_alloc(instance_id, size, alignof(struct AC), 0xAC);
    memcpy(address, &ac, size);
    struct AC* ptrac = (struct AC*)address;
    print("-- STRUCT AC -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);

    size = sizeof(struct BC);
    address = queue_allocator_alloc(instance_id, size, alignof(struct BC), 0xBC);
    memcpy(address, &bc, size);
    struct BC* ptrbc = (struct BC*)address;
    print("-- STRUCT BC -- %s size32:%u %s\n", COLOR_ARRAY_GET(color_index++), size/SIZE_I32, CLEAR);



    /* test allocator: pop/dequeue */
    uint32 total_alloc_size = 0;
    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_release(instance_id);
    memset(address, 0xA, size); /// mark memory

    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_release(instance_id);
    memset(address, 0xB, size); /// mark memory

    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_release(instance_id);
    memset(address, 0xC, size); /// mark memory

    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_release(instance_id);
    memset(address, 0xD, size); /// mark memory

    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_release(instance_id);
    memset(address, 0xE, size); /// mark memory

    size = queue_allocator_get_last_elem_size(instance_id); total_alloc_size += size;
    address = queue_allocator_release(instance_id);
    memset(address, 0xF, size); /// mark memory


    /* print-debug memory */
    print("\n-- COMPARE THE SIZE OF EACH STRUCT WITH THE SIZE OF THE MEMORY FOOT PRINT --");
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


void
test_queue_allocator_alignment(void)
{
    void* address = NULL;
    const memid_t instance_id = queue_allocator_create_new_instance(4);
    char* start_address = debug_get_queue_allocator_instance_start_address(instance_id);
    uint32 total_size = 0;

    {
        total_size = 0;
        uint8  val1 = HACK_LIT_STR_TO_NUM("1", uint8);
        uint16 val2 = HACK_LIT_STR_TO_NUM("22", uint16);
        uint32 val3 = HACK_LIT_STR_TO_NUM("4444", uint32);
        uint64 val4 = HACK_LIT_STR_TO_NUM("88888888", uint64);
        
        total_size += sizeof(val1);
        total_size += sizeof(val2);
        total_size += sizeof(val3);
        total_size += sizeof(val4);
        
        memset(start_address, 0, total_size);
        
        print("-- TEST  integer types mixed --\n   total size:%u\n", total_size);
        
        address = queue_allocator_alloc(instance_id, sizeof(val1), sizeof(val1), 0xCD); memcpy(address, &val1, sizeof(val1));
        address = queue_allocator_alloc(instance_id, sizeof(val2), sizeof(val2), 0xCD); memcpy(address, &val2, sizeof(val2));
        address = queue_allocator_alloc(instance_id, sizeof(val3), sizeof(val3), 0xCD); memcpy(address, &val3, sizeof(val3));
        address = queue_allocator_alloc(instance_id, sizeof(val4), sizeof(val4), 0xCD); memcpy(address, &val4, sizeof(val4));
        
        /* test allocator: pop */
        uint64 val = 0;
        
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val4, typeof(val4));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val3, typeof(val3));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val2, typeof(val2));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val1, typeof(val1));
        
        /* print memory */
        print_mem_hex(total_size, start_address);
    }

    /***********************************************************************************************
     * REPEAT MIXED
     **********************************************************************************************/
    {
        total_size = 0;
        uint8  val1 = HACK_LIT_STR_TO_NUM("1", uint8);
        uint16 val2 = HACK_LIT_STR_TO_NUM("22", uint16);
        uint32 val3 = HACK_LIT_STR_TO_NUM("4444", uint32);
        uint64 val4 = HACK_LIT_STR_TO_NUM("88888888", uint64);
        
        total_size += sizeof(val1);
        total_size += sizeof(val2);
        total_size += sizeof(val3);
        total_size += sizeof(val4);
        
        memset(start_address, 0, total_size);
        
        print("-- TEST  integer types mixed --\n total size:%u\n", total_size);
        
        
        /* test allocator: push  */
        address = queue_allocator_alloc(instance_id, sizeof(val4), sizeof(val4), 0xCD); memcpy(address, &val4, sizeof(val4));
        address = queue_allocator_alloc(instance_id, sizeof(val3), sizeof(val3), 0xCD); memcpy(address, &val3, sizeof(val3));
        address = queue_allocator_alloc(instance_id, sizeof(val2), sizeof(val2), 0xCD); memcpy(address, &val2, sizeof(val2));
        address = queue_allocator_alloc(instance_id, sizeof(val1), sizeof(val1), 0xCD); memcpy(address, &val1, sizeof(val1));
        address = queue_allocator_alloc(instance_id, sizeof(val1), sizeof(val1), 0xCD); memcpy(address, &val1, sizeof(val1));
        address = queue_allocator_alloc(instance_id, sizeof(val2), sizeof(val2), 0xCD); memcpy(address, &val2, sizeof(val2));
        address = queue_allocator_alloc(instance_id, sizeof(val1), sizeof(val1), 0xCD); memcpy(address, &val1, sizeof(val1));
        address = queue_allocator_alloc(instance_id, sizeof(val3), sizeof(val3), 0xCD); memcpy(address, &val3, sizeof(val3));
        address = queue_allocator_alloc(instance_id, sizeof(val1), sizeof(val1), 0xCD); memcpy(address, &val1, sizeof(val1));
        address = queue_allocator_alloc(instance_id, sizeof(val4), sizeof(val4), 0xCD); memcpy(address, &val4, sizeof(val4));
        
        /* test allocator: pop */
        uint64 val = 0;
        
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val4, typeof(val4));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val1, typeof(val1));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val3, typeof(val3));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val1, typeof(val1));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val2, typeof(val2));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val1, typeof(val1));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val1, typeof(val1));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val2, typeof(val2));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val3, typeof(val3));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val4, typeof(val4));
        
        /* print memory */
        print_mem_hex(total_size, start_address);
    }


    /***********************************************************************************************
     * REPEAT MIXED
     **********************************************************************************************/
    {
        total_size = 0;
        uint8  val1 = HACK_LIT_STR_TO_NUM("1", uint8);
        uint16 val2 = HACK_LIT_STR_TO_NUM("22", uint16);
        uint32 val3 = HACK_LIT_STR_TO_NUM("4444", uint32);
        uint64 val4 = HACK_LIT_STR_TO_NUM("88888888", uint64);
        char buf16[11] = "123456789a\0";
        
        total_size += sizeof(val1);
        total_size += sizeof(val2);
        total_size += sizeof(val3);
        total_size += sizeof(val4);
        total_size += sizeof(buf16);
        
        memset(start_address, 0, total_size);
        
        print("-- TEST integer types mixed--\n   total size:%u\n", total_size);
        
        address = queue_allocator_alloc(instance_id, sizeof(val1), sizeof(val1), 0xCD); memcpy(address, &val1, sizeof(val1));
        address = queue_allocator_alloc(instance_id, sizeof(val3), sizeof(val3), 0xCD); memcpy(address, &val3, sizeof(val3));
        address = queue_allocator_alloc(instance_id, sizeof(val2), sizeof(val2), 0xCD); memcpy(address, &val2, sizeof(val2));
        address = queue_allocator_alloc(instance_id, sizeof(buf16), sizeof(char), 0xCD); memcpy(address, &buf16, sizeof(buf16));
        address = queue_allocator_alloc(instance_id, sizeof(val4), sizeof(val4), 0xCD); memcpy(address, &val4, sizeof(val4));
        
        /* test allocator: pop */
        uint64 val = 0;
        
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val4, typeof(val4));
        address = queue_allocator_release(instance_id);
        // test_queue_equality_value(address, val, buf16, typeof(char));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val2, typeof(val2));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val3, typeof(val3));
        address = queue_allocator_release(instance_id);
        test_queue_equality_value(address, val, val1, typeof(val1));
        
        /* print memory */
        print_mem_hex(total_size, start_address);
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
        
        print("-- TEST char buffers --\n   total size:%u\n", total_size);
        
        
        address = queue_allocator_alloc(instance_id, sizeof(buf1), sizeof(char), 0xCD); memcpy(address, buf1, sizeof(buf1));
        address = queue_allocator_alloc(instance_id, sizeof(buf2), sizeof(char), 0xCD); memcpy(address, buf2, sizeof(buf2));
        address = queue_allocator_alloc(instance_id, sizeof(buf3), sizeof(char), 0xCD); memcpy(address, buf3, sizeof(buf3));
        address = queue_allocator_alloc(instance_id, sizeof(buf4), sizeof(char), 0xCD); memcpy(address, buf4, sizeof(buf4));
        
        
        /* test allocator: pop */
        address = queue_allocator_release(instance_id);
        test_queue_equality_string(address, buf4, char);
        address = queue_allocator_release(instance_id);
        test_queue_equality_string(address, buf3, char);
        address = queue_allocator_release(instance_id);
        test_queue_equality_string(address, buf2, char);
        address = queue_allocator_release(instance_id);
        test_queue_equality_string(address, buf1, char);
        
        
        /* print memory */
        print_mem_hex(total_size, start_address);
    }


    /***********************************************************************************************
     * MIXED: uint32 buffers
     **********************************************************************************************/
    {
        total_size = 0;
        uint8  buf1[6] = {0};
        uint32 buf2[2] = {0};
        uint8  buf3[2] = {0};
        uint64 buf4[1] = {0};

        char* ptr = (char*)buf1;
        uint32 i = 0;
        for(; i < sizeof(buf1)-1; i++) { ptr[i] = 'a'; } ptr[i] = '\0'; i = 0; ptr = (char*)buf2;
        for(; i < sizeof(buf2)-1; i++) { ptr[i] = 'b'; } ptr[i] = '\0'; i = 0; ptr = (char*)buf3;
        for(; i < sizeof(buf3)-1; i++) { ptr[i] = 'c'; } ptr[i] = '\0'; i = 0; ptr = (char*)buf4;
        for(; i < sizeof(buf4)-1; i++) { ptr[i] = 'd'; } ptr[i] = '\0'; i = 0;

        total_size += sizeof(buf1);
        total_size += sizeof(buf2);
        total_size += sizeof(buf3);
        total_size += sizeof(buf4);

        memset(start_address, 0, total_size);

        print("-- TEST uint32 buffers --\n   total size:%u\n", total_size);


        address = queue_allocator_alloc(instance_id, sizeof(buf1), sizeof(uint8 ), 0xCD); memcpy(address, buf1, sizeof(buf1));
        address = queue_allocator_alloc(instance_id, sizeof(buf2), sizeof(uint32), 0xCD); memcpy(address, buf2, sizeof(buf2));
        address = queue_allocator_alloc(instance_id, sizeof(buf3), sizeof(uint8 ), 0xCD); memcpy(address, buf3, sizeof(buf3));
        address = queue_allocator_alloc(instance_id, sizeof(buf4), sizeof(uint64), 0xCD); memcpy(address, buf4, sizeof(buf4));
        /* again, insert the first 2 buffers */
        address = queue_allocator_alloc(instance_id, sizeof(buf1), sizeof(uint8 ), 0xCD); memcpy(address, buf1, sizeof(buf1));
        address = queue_allocator_alloc(instance_id, sizeof(buf2), sizeof(uint32), 0xCD); memcpy(address, buf2, sizeof(buf2));

        /* test allocator: pop */
        address = queue_allocator_release(instance_id);
        test_queue_equality_buffer(address, buf2, uint32);
        address = queue_allocator_release(instance_id);
        test_queue_equality_buffer(address, buf1, uint8);
        address = queue_allocator_release(instance_id);
        test_queue_equality_buffer(address, buf4, uint64);
        address = queue_allocator_release(instance_id);
        test_queue_equality_buffer(address, buf3, uint8);
        address = queue_allocator_release(instance_id);
        test_queue_equality_buffer(address, buf2, uint32);
        address = queue_allocator_release(instance_id);
        test_queue_equality_buffer(address, buf1, uint8);

        /* print memory */
        print_mem_hex(total_size, start_address);
    }


    
}
