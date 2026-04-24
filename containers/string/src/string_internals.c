/*
 * Created by ckalo on 3/8/26.
 */

#include "string_internals.h"
#include <assert.h>
#include "string_struct.h"
#include "file_sys_interface.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include "slab_allocator_internals.h"
#include "slab_allocator.h"
#include "stringc.h"
#include "stack_allocator.h"


/*******************************************************************************
 *
 ******************************************************************************/
static uint32 string_alloced_count = 0;
static memid_t allocator_instance_id = -1; // MAX_UINT for ERROR

static inline strlen_t string_get_self_size() { return sizeof(string_t) - sizeof(char); }


void
string_container_init(const pageid_t reserve_num_pages)
{
    allocator_instance_id = slab_allocator_create_new_instance(reserve_num_pages, "string_container_init()");
    // allocator_instance_id = stack_allocator_create_new_instance(4);
}

void
string_container_destroy(void)
{
    print("FREEING strings: calling string_mem_cache_table_destroy() from string container.\n");
    // string_mem_cache_table_destroy();
    mcache_table_destroy();
}

/***************************************************************************************************
 * string constructors-destructors
 **************************************************************************************************/
string_t*
string_constructor(const char* str, const strlen_t size)
{
    const strlen_t len = strlen(str);
    const strlen_t self_size = string_get_self_size();

    slab_size_t capacity = (size > STRING_MIN_LEN) ? size : len + self_size + 1;

    strlen_t max_size = STRING_MIN_LEN;
    while(max_size < capacity) max_size *= 2;

    /* alloc storage for bufer data */
    void* data_ptr = NULL;

    assert(allocator_instance_id != (uint16)INT_NULL);
    data_ptr = slab_allocator_alloc(allocator_instance_id, max_size, sizeof(char), "string_constructor()");
    // data_ptr = stack_allocator_alloc(allocator_instance_id, max_size);

    string_t* string = data_ptr;
    string->len = 0;
    string->size = max_size;
    string->self_size = self_size;

    /* set string data */
    string_memset(string, 0x0);
    string_set_cstr(&string, str);

    string_alloced_count++;

    return string;
}

void
string_destructor(string_t* string)
{
    // Release string's data slab
    // string_slab_alloc_release(string->data, string->size);
    slab_allocator_release(allocator_instance_id, string->data, string->size, sizeof(char));

    /* DEBUG-FPRINT
     * write a string to string's->data memory
     * CAN COLOR MEMORY FOR SLAB SIZE > 8 BYTES */
    if(string->len > 8) {
        char_buf16_t dinfo;
        char_buf16_copy_from_str(&dinfo, "EMPTY");
        // char inttostr[INT32_TO_STR_BUFFER_SIZE];
        // str_conv_int32_to_str(inttostr, string->id);
        // str_concat_memcpy(dinfo.buf, inttostr, 16);
        char* writable_data = string->data + sizeof(char*); // offset of size *pointer for slabs free list
        while(string->len) {
            memcpy((void*)writable_data, dinfo.buf, sizeof(char*));
            writable_data += sizeof(char);
            string->len--;
        }
    }
    /* DEBUG-FPRINT */

    string->data[0] = '\0';
    string->len = 0;

    string_alloced_count--;
}

void
string_release(string_t* string)
{
    /* Release string's data (slab) */
    // string_slab_alloc_release(string->data, string->size);
    slab_allocator_release(allocator_instance_id, string->data, string->size, sizeof(char));


    /* DEBUG-FPRINT
     * write a string to string's->data memory
     */
    char_buf16_t dinfo;
    char_buf16_copy_from_str(&dinfo, (const char*)"DESTROYED ID:");
    // char inttostr[INT32_TO_STR_BUFFER_SIZE];
    // str_conv_int32_to_str(inttostr, string->id);
    // str_concat_memcpy(dinfo.buf, inttostr, 16);
    memcpy((void*)string->data, dinfo.buf, sizeof(char*) * 2);

    string->data[0] = '\0';
    string->len = 0;

    // free string from string table
    string_alloced_count--;
}


string_t*
string_reallocate(string_t* string, const strlen_t len, bool copy)
{

    const uint16 new_len = string->len + len + string->self_size+1;
    assert(new_len > sizeof(strlen_t));
    print("new len:%u\n", new_len);

    strlen_t max_size = STRING_MIN_LEN;
    while(max_size <= new_len) {
        assert(max_size > 0);
        max_size *= 2;
    }


    assert("IMPLEMENT realloc() for stack allocator" > 0);
    void* data_ptr = NULL;
    data_ptr = slab_allocator_alloc(allocator_instance_id, max_size, sizeof(char), "string_reallocate()");
    // stack_allocator_release(allocator_instance_id);
    // data_ptr = stack_allocator_alloc(allocator_instance_id, max_size, sizeof(char));


    strlen_t old_len = string->len;
    strlen_t old_self_size = string->self_size;
    memset((void*)data_ptr, 0x0, max_size);
    if(copy) memcpy((void*)data_ptr, string, string->len + string->self_size);

    slab_allocator_release(allocator_instance_id, string->data, string->size, sizeof(char));
    // stack_allocator_release(allocator_instance_id);

    string = data_ptr;
    string->len = old_len;
    string->self_size = old_self_size;
    string->size = max_size;

    return string;
}

/***************************************************************************************************
 * PRIVATE
 **************************************************************************************************/
void
string_memcopy(string_t* const dst, const string_t* const src)
{
    memcpy((void*)dst->data, src->data, src->len * sizeof(string_t));
}

void
string_memset(const string_t* const string, const uint8 val)
{
    memset((void*)&string->data[0], val, string->size-string->self_size);
}
