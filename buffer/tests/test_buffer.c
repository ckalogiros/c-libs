/*
 * Created by ckalo on 1/8/26.
 */

#include "test_buffer.h"
#include "buffer.h"
#include "buffer_log.h"
#include "file_sys_debug.h"
#include "file_sys_interface.h"
#include "logger.h"
#include "mem_man_interface.h"
#include "slab_allocator_internals.h"
#include "string_format.h"

#define DEBUG_FPRINT_ALL_ALLOC_MEM()\
const char* fname = "/exit_status_ok_debug.txt";\
struct fs_t* fs = fs_create(fname);\
fs_fprint_all_allocated_mem(fname);\
fs_append(fs, "EXIT WITH STATUS OK\n")


/*
 * operations-helpers
 */
void
fill(buffer_t* const buffer)
{
    uint32* data = buffer->data;
    for(uint32 i = 0; i < buffer->capacity; i++) {
        // buffer_push_uint32(i+1, buffer);
        buffer_set_for_push(buffer);
        data[i] = i + 1;
        file_sys_push(buffer->data, sizeof(data[i]), TYPE_UINT32, buffer->data);
    }
}

void
insert_at(const uint32 val, buffer_t* const buffer, const uint32 index)
{
    print("%s-- INSERT ELEMENT AT INDEX: %d%s\n\n", gc, index, CLEAR);
    if(index != INT_NULL)
        buffer_insert_uint32(val, buffer, index);
}

void
remove_from(buffer_t* const buffer, const uint32 index)
{
    print("%s-- REMOVE ELEMENT AT INDEX:%d%s\n\n", gc, index, CLEAR);
    buffer_free_elem(buffer, index);
}

void
test_create_buffer(const uint32 capacity)
{
    memman_init();

    buffer_table_init();

    // allocate new buffer
    buffer_t* buffer = buffer_create_uint32(capacity);

    fill(buffer);

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));

    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}

void
test_destroy_buffer()
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s buffer destroy - (no slabs realease)%s\n\n", CYAN, CLEAR);

    memman_init();

    buffer_table_init();

    print("%sTEST CREATE-DESTROY,\nSAME BUFFER TWICE%s\n", GREEN, CLEAR);
    {
        buffer_t* buffer = buffer_create_uint32(16);
        const buffer_t* buffer_address = buffer;
        fill(buffer);

        print("\n%sBEFORE RECREATING%s\n", CYAN, CLEAR);
        print("\n%sTEST REMOVING: 0,3,6,9,12 indices%s\n", GREEN, CLEAR);
        buffer_free_elem(buffer, 0);
        buffer_free_elem(buffer, 3);
        buffer_free_elem(buffer, 6);
        buffer_free_elem(buffer, 9);
        buffer_free_elem(buffer, 12);
        buffer_print_free_list(buffer);

        buffer_destroy(buffer);

        // recreate buffer
        {
            buffer = buffer_create_uint32(16);
            fill(buffer);

            if(buffer_address == buffer)
                print("%sTEST PASSED%s\n", GREEN, CLEAR);
            else
                print("%sTEST FAILED%s\n", RED, CLEAR);
            print(
                "buffer1 %saddr:%x%s\n"
                "buffer2 %saddr:%x%s\n",
                GREEN, buffer_address, CLEAR,
                GREEN, buffer, CLEAR
            );
        }

        buffer_destroy(buffer);

        // recreate buffer
        {
            buffer = buffer_create_uint32(16);
            fill(buffer);

            if(buffer_address == buffer)
                print("%sTEST PASSED%s\n", GREEN, CLEAR);
            else
                print("%sTEST FAILED%s\n", RED, CLEAR);
            print(
                "buffer1 %saddr:%x%s\n"
                "buffer2 %saddr:%x%s\n",
                GREEN, buffer_address, CLEAR,
                GREEN, buffer, CLEAR
            );
        }

        print("\n%sAFTER RECREATING%s\n", CYAN, CLEAR);
        print("\n%sTEST REMOVING: 0,3,6,9,12 indices%s\n", GREEN, CLEAR);
        buffer_free_elem(buffer, 0);
        buffer_free_elem(buffer, 3);
        buffer_free_elem(buffer, 6);
        buffer_free_elem(buffer, 9);
        buffer_free_elem(buffer, 12);
        buffer_print_free_list(buffer);
    }

    print("\n\n%sTEST CREATE-DESTROY,\n"
          "create 2 buffers,\n"
          "destroy 2 buffer,\n"
          "recreate 2 buffers%s\n", GREEN, CLEAR);
    {
        buffer_t* buffer1 = buffer_create_uint32(16);
        const buffer_t* buffer1_address = buffer1;
        fill(buffer1);

        buffer_t* buffer2 = buffer_create_uint32(16);
        const buffer_t* buffer2_address = buffer2;
        fill(buffer2);

        print(
            "buffer1 %saddr:%x%s\n"
            "buffer2 %saddr:%x%s\n",
            GREEN, buffer1_address, CLEAR,
            GREEN, buffer2_address, CLEAR
        );

        {
            print("\n%sBEFORE RECREATING%s\n", CYAN, CLEAR);
            print("\n%sTEST REMOVING: 0,3,6,9,12 indices%s\n", GREEN, CLEAR);
            buffer_free_elem(buffer1, 0);
            buffer_free_elem(buffer1, 3);
            buffer_free_elem(buffer1, 6);
            buffer_free_elem(buffer1, 9);
            buffer_free_elem(buffer1, 12);
            buffer_print_free_list(buffer1);

            print("\n%sTEST REMOVING: 7,9,11,13,15 indices%s\n", GREEN, CLEAR);
            buffer_free_elem(buffer2, 7);
            buffer_free_elem(buffer2, 9);
            buffer_free_elem(buffer2, 11);
            buffer_free_elem(buffer2, 13);
            buffer_free_elem(buffer2, 15);
            buffer_print_free_list(buffer2);
        }


        buffer_destroy(buffer1);
        buffer_destroy(buffer2);

        // recreate buffer1
        {
            buffer1 = buffer_create_uint32(16);
            fill(buffer1);
            if(buffer2_address == buffer1)
                print("%sTEST PASSED%s\n", GREEN, CLEAR);
            else
                print("%sTEST FAILED%s\n", RED, CLEAR);
            print(
                "buffer1 %saddr:%x%s\n"
                "buffer1 %saddr:%x%s\n",
                GREEN, buffer1, CLEAR,
                GREEN, buffer2_address, CLEAR
            );
        }


        // recreate buffer2
        {
            buffer2 = buffer_create_uint32(16);
            fill(buffer2);
            if(buffer1_address == buffer2)
                print("%sTEST PASSED%s\n", GREEN, CLEAR);
            else
                print("%sTEST FAILED%s\n", RED, CLEAR);
            print(
                "buffer2 %saddr:%x%s\n"
                "buffer2 %saddr:%x%s\n",
                GREEN, buffer2, CLEAR,
                GREEN, buffer1_address, CLEAR
            );
        }

        {
            print("\n%sBEFORE RECREATING%s\n", CYAN, CLEAR);
            print("\n%sTEST REMOVING: 0,3,6,9,12 indices%s\n", GREEN, CLEAR);
            buffer_free_elem(buffer1, 0);
            buffer_free_elem(buffer1, 3);
            buffer_free_elem(buffer1, 6);
            buffer_free_elem(buffer1, 9);
            buffer_free_elem(buffer1, 12);
            buffer_print_free_list(buffer1);

            print("\n%sTEST REMOVING: 7,9,11,13,15 indices%s\n", GREEN, CLEAR);
            buffer_free_elem(buffer2, 7);
            buffer_free_elem(buffer2, 9);
            buffer_free_elem(buffer2, 11);
            buffer_free_elem(buffer2, 13);
            buffer_free_elem(buffer2, 15);
            buffer_print_free_list(buffer2);
        }
    }


    print("\n\n%sTEST FILL ALL SLABS,\n"
          "create that many buffers of total mem_cache slabs,\n"
          "destroy all buffers,\n"
          "recreate all buffers%s\n", GREEN, CLEAR);
    {
        const uint32 capacity = 32; // create 8 total slabs per page
        buffer_t* buffers[capacity];
        const uint32 typesize = sizeof(uint32);
        const uint32 buffers_per_page = PAGE_SIZE / (capacity * typesize);

        // CREATE
        for(uint32 i = 0; i < buffers_per_page; i++) {
            buffers[i] = buffer_create_uint32(capacity);
            fill(buffers[i]);

            print(
                "buffers[i] %saddr:%x%s\n",
                GREEN, buffers[i]->data, CLEAR
            );
        }

        // DESTROY
        for(uint32 i = 0; i < buffers_per_page; i++) {
            buffer_destroy(buffers[i]);
        }


        // RE-CREATE
        buffer_t* recreated[capacity];
        for(uint32 i = 0; i < buffers_per_page; i++) {
            recreated[i] = buffer_create_uint32(capacity);
            fill(recreated[i]);

            print(
                "recreated[i] %saddr:%x%s\n",
                GREEN, recreated[i]->data, CLEAR
            );
        }

        for(uint32 i = 0; i < buffers_per_page; i++) {
            if(buffers[i] == recreated[(buffers_per_page - 1) - i])
                print("buffer:[%u] %sTEST PASSED%s\n", i, GREEN, CLEAR);
            else
                print("buffer:[%u] %sTEST FAILED%s\n", i, RED, CLEAR);
        }
    }


    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}

void
test_buffer_realloc()
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s buffer realloc%s\n\n", CYAN, CLEAR);

    memman_init();

    buffer_table_init();

    // allocate buffer
    const uint32 capacity = 16;
    print("CREATE %scapacity:%u%s\n", GREEN, capacity, CLEAR);
    buffer_t* buffer = buffer_create_gentype(capacity, sizeof(uint32));
    fill(buffer); // fill buffer
    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));


    // force buffer realloc - to capacity:32
    buffer_push_uint32(100, buffer);
    print("REALLOC buffer %scapacity:%u%s\n", GREEN, buffer->capacity, CLEAR);
    for(uint32 i = 0; i < buffer->capacity / 2; i++) {
        buffer_push_uint32(100 + i, buffer);
    }

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));


    // force buffer realloc - to capacity:64
    buffer_push_uint32(200, buffer);
    print("REALLOC buffer %scapacity:%u%s\n", GREEN, buffer->capacity, CLEAR);
    for(uint32 i = 0; i < buffer->capacity / 2; i++) {
        buffer_push_uint32(200 + i, buffer);
    }

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));


    // force buffer realloc - to capacity:128
    buffer_push_uint32(300, buffer);
    print("REALLOC buffer %scapacity:%u%s\n", GREEN, buffer->capacity, CLEAR);
    for(uint32 i = 0; i < buffer->capacity / 2; i++) {
        buffer_push_uint32(300 + i, buffer);
    }

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));


    // force buffer realloc - to capacity:256
    buffer_push_uint32(400, buffer);
    print("REALLOC buffer %scapacity:%u%s\n", GREEN, buffer->capacity, CLEAR);
    for(uint32 i = 0; i < buffer->capacity / 2; i++) {
        buffer_push_uint32(400 + i, buffer);
    }

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));


    // force buffer realloc - to capacity:512
    buffer_push_uint32(400, buffer);
    print("REALLOC buffer %scapacity:%u%s\n", GREEN, buffer->capacity, CLEAR);
    for(uint32 i = 0; i < buffer->capacity / 2; i++) {
        buffer_push_uint32(400 + i, buffer);
    }

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));


    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}

void
test_buffer_fill_remove_insert()
{
    memman_init();

    buffer_table_init();

    // allocate new buffer
    const uint32 capacity = 64;
    buffer_t* buffer = buffer_create_uint32(capacity);

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));


    // fill buffer
    fill(buffer);


    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));

    const int32 index = 9;

    // remove an element, using free buffer to store all free buffer elems
    remove_from(buffer, index);
    remove_from(buffer, 63);
    remove_from(buffer, 10);
    remove_from(buffer, 11);
    remove_from(buffer, 12);

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));

    // insert a value in the buffer at specific index, using the free buffer 'find' a free element
    insert_at(333333, buffer, index);
    insert_at(333333, buffer, 10);
    insert_at(333333, buffer, 11);
    insert_at(333333, buffer, 12);

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));

    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}

void
test_buffer_different_capacity_buffers()
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s create different capacity buffers%s\n\n", CYAN, CLEAR);

    memman_init();

    buffer_table_init();

    // allocate buffer: capacity=16
    {
        const uint32 capacity = 16;
        print("CREATE %scapacity:%u%s\n", GREEN, capacity, CLEAR);
        buffer_t* buffer = buffer_create_uint32(capacity);
        fill(buffer); // fill buffer
        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer, .print_num_elems = buffer->capacity
        }));
    }

    // allocate buffer: capacity=32
    {
        const uint32 capacity = 32;
        print("CREATE %scapacity:%u%s\n", GREEN, capacity, CLEAR);
        buffer_t* buffer = buffer_create_uint32(capacity);
        fill(buffer); // fill buffer
        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer, .print_num_elems = buffer->capacity
        }));
    }

    {
        const uint32 capacity = 16;
        print("CREATE %scapacity:%u%s\n", GREEN, capacity, CLEAR);
        buffer_t* buffer = buffer_create_uint32(capacity);
        fill(buffer); // fill buffer
        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer, .print_num_elems = buffer->capacity
        }));
    }

    {
        const uint32 capacity = 16;
        print("CREATE %scapacity:%u%s\n", GREEN, capacity, CLEAR);
        buffer_t* buffer = buffer_create_uint32(capacity);
        fill(buffer); // fill buffer
        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer, .print_num_elems = buffer->capacity
        }));
    }

    {
        const uint32 capacity = 16;
        print("CREATE %scapacity:%u%s\n", GREEN, capacity, CLEAR);
        buffer_t* buffer = buffer_create_uint32(capacity);
        fill(buffer); // fill buffer
        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer, .print_num_elems = buffer->capacity
        }));
    }


    // allocate buffer: capacity=64
    {
        const uint32 capacity = 64;
        print("CREATE %scapacity:%u%s\n", GREEN, capacity, CLEAR);
        buffer_t* buffer = buffer_create_uint32(capacity);
        fill(buffer); // fill buffer
        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer, .print_num_elems = buffer->capacity
        }));
    }

    // allocate buffer: capacity=256
    {
        const uint32 capacity = 256;
        print("CREATE %scapacity:%u%s\n", GREEN, capacity, CLEAR);
        buffer_t* buffer_c256 = buffer_create_uint32(capacity);
        fill(buffer_c256); // fill buffer
        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer_c256, .print_num_elems = buffer_c256->capacity
        }));
    }

    // allocate buffer: capacity=512
    {
        const uint32 capacity = 512;
        print("CREATE %scapacity:%u%s\n", GREEN, capacity, CLEAR);
        buffer_t* buffer = buffer_create_uint32(capacity);
        fill(buffer); // fill buffer
        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer, .print_num_elems = buffer->capacity
        }));
    }

    buffer_table_destroy();

    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}

void
test_buffer_remove_elements(const uint32 capacity)
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s buffer remove elements %s\n\n", CYAN, CLEAR);

    memman_init();

    buffer_table_init();

    // allocate buffer
    buffer_t* buffer = buffer_create_uint32(capacity);
    fill(buffer); // fill buffer


    // remove elements and print free list
    uint32 k = 4;
    print("%sREMOVE%s on every %uth bit\n", GREEN, CLEAR, k);
    for(uint32 i = 0; i < 256; i++) {
        if(i % 64 == 0 && i != 0) {
            k *= 2;
            print("%sREMOVE%s on every %uth bit\n", GREEN, CLEAR, k);
        }
        if(i % k) {
            buffer_free_elem(buffer, i);
        }
    }

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));

    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}

void
test_buffer_insert_elements(const uint32 capacity)
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s buffer remove elements %s\n\n", CYAN, CLEAR);

    memman_init();

    buffer_table_init();

    // allocate buffer
    buffer_t* buffer = buffer_create_gentype(capacity, sizeof(uint32));


    // remove elemnts and print free list
    uint32 k = 4;
    print("%sINSERT%s on every %uth bit\n", GREEN, CLEAR, k);
    for(uint32 i = 0; i < 256; i++) {
        if(i % 64 == 0 && i != 0) {
            k *= 2;
            print("%sINSERT%s on every %uth bit\n", GREEN, CLEAR, k);
        }
        if(i % k) {
            buffer_insert_uint32(i, buffer, i);
        }
    }


    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));


    buffer_fprint(buffer);

    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}

void
test_generic_type_buffer()
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s test_generic_type_buffer%s\n\n", CYAN, CLEAR);

    memman_init();

    buffer_table_init();

    buffer_t* buffer = buffer_create_gentype(32, sizeof(uint8));

    // fill buffer
    for(uint32 i = 0; i < buffer->capacity; i++) {
        const uint64 val = 0xffffffffffffffff;
        // const uint64 val = 14;
        uint8* data = buffer->data;
        const uint16 idx = buffer_set_for_push(buffer);
        data[idx] = val;
        // buffer_push_gentype_macro(val, buffer);
    }


    // print
    const uint8* ptr = (uint8*) buffer->data;
    for(uint32 i = 0; i < buffer->capacity; i++) {
        const uint32 index = i * buffer->typesize;
        print(
            "[%s%u%s]=%s%u%s, ",
            YELLOW, i, CLEAR,
            CYAN, *(ptr + index), CLEAR
        );

        if((i + 1) % 3 == 0) print("\n    ");
    }


    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}

void
test_buffer_free_elem()
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s test_buffer_find_tail%s\n\n", CYAN, CLEAR);

    memman_init();

    buffer_table_init();

    const uint32 capacity = 128;
    const uint32 type_size = sizeof(uint32);
    buffer_t* buffer = buffer_create_gentype(capacity, type_size);

    // fill buffer
    for(uint32 i = 0; i < buffer->capacity; i++) {
        if(i % 3 == 0) {
            buffer_set_for_insert(buffer, i);
            uint32* data = buffer->data;
            data[i] = i + 1;
        }
    }

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));

    for(uint32 i = 0; i < buffer->capacity; i++) {
        buffer_free_elem(buffer, i);
    }

    buffer_print_uint32(buffer_print_config((buffer_print_params){
        .buffer = buffer, .print_num_elems = buffer->capacity
    }));

    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}

void // todo
test_buffer_find_tail()
{}

void // todo
test_buffer_get_next_free()
{}


/***************************************************************************************************
 * buffers inside of buffers !
 **************************************************************************************************/
void
test_buffer_with_children()
{
    print("%s-- -- -- TEST: -- -- --%s\n", GREEN, CLEAR);
    print("%s test_buffer_with_children%s\n\n", CYAN, CLEAR);

    memman_init();

    buffer_table_init();

    struct node {
        uint32 nameid;
        buffer_t* children;
    };

    const uint32 capacity = 4;
    const uint32 type_size = sizeof(struct node);
    buffer_t* buffer = buffer_create_gentype(capacity, type_size);
    print("add buffer at:%x\n", REDUCE_ADDR(buffer->data, F6));
    // fill buffer
    for(uint32 i = 0; i < buffer->capacity; i++) {
        buffer_set_for_insert(buffer, i);
        struct node* data = buffer->data;
        data[i].nameid = i + 1;

        data[i].children = buffer_create_gentype(capacity, type_size);
        print("add child at:%x\n", REDUCE_ADDR(data[i].children->data, F6));
        // for(uint32 j = 0; j < data[i].children->capacity; j++) {
        for(uint32 j = 0; j < capacity; j++) {
            buffer_set_for_insert(data[i].children, j);
            struct node* cdata = data[i].children->data;
            cdata[j].nameid = j + 1;
            cdata[j].children = NULL;
        }
    }

    for(uint32 i = 0; i < buffer->capacity; i++) {
        const struct node* data = (struct node*) buffer->data;
        print("elem:[%u]: id:%u\n", i, data[i].nameid);

        for(uint32 j = 0; j < data[i].children->capacity; j++) {
            const struct node* cdata = data[i].children->data;
            print("  child:[%u]: id:%u\n", j, cdata[j].nameid);
        }
    }


    // buffer_print_uint32(buffer_print_config((buffer_print_params){
    //     .buffer = buffer, .print_num_elems = buffer->capacity
    // }));

    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}


/***************************************************************************************************
 * test file print buffer values
 **************************************************************************************************/
void
test_buffer_fprint_values()
{
    memman_init();

    buffer_table_init();

    struct fs_t* fsdebug = fs_create("debug_with_info");

// #define TEST_UINT32
#ifdef TEST_UINT32
    { // uint32 values
        const uint32 capacity = 64;
        buffer_t* buffer = buffer_create_uint32(capacity);

        // fill(buffer);
        uint32* data = buffer->data;
        for(uint32 i = 0; i < buffer->capacity; i++) {
            buffer_set_for_push(buffer);
            data[i] = i + 1;
            file_sys_push(
                (char*) buffer->data + sizeof(uint32) * i,
                sizeof(uint32),
                TYPE_UINT32,
                (char*) buffer->data + sizeof(uint32) * i);
        }

        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer, .print_num_elems = buffer->capacity
        }));

        char buf[128] = {0};
        string_format(buf, "FLOATING POINT values\nbuffer capacity:%u\n", buffer->capacity);
        fs_append_mem_with_msg(fsdebug, buffer->data, buffer->capacity * buffer->typesize, buf);
    }
#endif
#undef TEST_UINT32

// #define TEST_FLOAT
#ifdef TEST_FLOAT
    { // float values
        const uint32 capacity = 64;
        buffer_t* buffer = buffer_create_gentype(capacity, sizeof(float));

        // fill(buffer)
        float val = .01;
        float* data = buffer->data;
        for(uint32 i = 0; i < buffer->capacity; i++) {
            buffer_set_for_push(buffer);
            data[i] = val;
            val += .01;
            file_sys_push(
                (char*) buffer->data + sizeof(float) * i,
                sizeof(float),
                TYPE_FLOAT,
                (char*) buffer->data + sizeof(float) * i);
        }

        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer, .print_num_elems = buffer->capacity
        }));

        char buf[128] = {0};
        string_format(buf, "FLOATING POINT values\nbuffer capacity:%u\n", buffer->capacity);
        fs_append_mem_with_msg(fsdebug, buffer->data, buffer->capacity * buffer->typesize, buf);
    }
#endif
#undef TEST_FLOAT

#define TEST_CHAR_ARRAY
#ifdef TEST_CHAR_ARRAY
    { // float values
        const uint32 capacity = 128;
        buffer_t* buffer = buffer_create_gentype(capacity, sizeof(char));

        // fill(buffer)
        char c = ' ';
        char* data = buffer->data;
        for(uint32 i = 0; i < buffer->capacity; i++) {
            buffer_set_for_push(buffer);
            data[i] = c;
            c += 1;
            file_sys_push(
                (char*) buffer->data + (sizeof(char) * i),
                sizeof(char),
                TYPE_CHAR,
                (char*) buffer->data + (sizeof(char) * i));
        }

        buffer_print_uint32(buffer_print_config((buffer_print_params){
            .buffer = buffer, .print_num_elems = buffer->capacity
        }));

        char buf[128] = {0};
        string_format(buf, "FLOATING POINT values\nbuffer capacity:%u\n", buffer->capacity);
        fs_append_mem_with_msg(fsdebug, buffer->data, buffer->capacity * buffer->typesize, buf);
    }
#endif
#undef TEST_CHAR_ARRAY

    DEBUG_FPRINT_ALL_ALLOC_MEM();
    memman_terminate();
}
