/*
 * Created by ckalo on 1/11/26.
 */

#include "buffer_arr_internals.h"
#include "bits.h"
#include <stdlib.h>
#include <string.h>
#include "buffer_arr_log.h"
#include "logger.h"
#include "sys_utils.h"
#include "slab_allocator_interface.h"
#include "file_sys_interface.h"
#include "slab_allocator_internals.h"
#include "str_utils.h"
#include "char_buf.h"


/***************************************************************************************************
 * FOR DEBUG */
/* output specific buffers to file */
static struct fs_t* fs_buffer = NULL;
/* output one debug file and register all created buffers  */
static struct fs_t* fs_register_created_buffers = NULL;
// todo: must be dynamicaly reallocated

#define BUFFER_ARR_TABLE_MIN_CAPACITY 512
#define BUFFER_ARR_ALIGNMENT 1024


#define DEBUG_FPRINT_BUFFER 1
#ifdef DEBUG_FPRINT_BUFFER
/* append info on buffer creation */
#define fs_register_buffer_arr_create(fs, buffer)\
fs_append(fs,\
"CREATE  : | "             \
" id:    %3u | "         \
" tsize: %5u | "          \
" capac: %5u slabs | "          \
" count: %3u | "          \
" tail:  %3d | "          \
" data:  %08x %08x " \
" flist: %08x %08x\n", \
buffer->id,       \
buffer->typesize, \
buffer->capacity, \
buffer->count,    \
buffer->tail,     \
(uint64)buffer->data >> 32,(uint64)buffer->data & 0xFFFFFFFF, \
(uint64)buffer->free_list >> 32,(uint64)buffer->free_list & 0xFFFFFFFF \
)

/* append formated data to the buffer's file */
#define fs_buffer_arr_append(fs, fmt, ...)\
fs_append(fs, fmt, ##__VA_ARGS__)

#else

#endif
/* FOR DEBUG
 **************************************************************************************************/


/*******************************************************************************
 * buffer_table: a buffer to store all applications buffer structures
 ******************************************************************************/
/* an array of pointers of type bufidx_t, that the [0] elem is the pointer to memory block start */
buffer_t* buffer_table = NULL;
uint32 buffer_table_count = 0;
uint32 ID = 0;
uint16 allocator_instance_id = 0;
uint16 buffer_arr_get_allocator_instance_id(){ return allocator_instance_id; }


uint32 buffer_arr_get_id(){ return ++ID; }



void
TEMP_PRINT_BUFFER_ARR_TABLE_COUNT()
{
    print("TEMP_PRINT_BUFFER_ARR_TABLE_COUNT:%u\n", buffer_table_count);
}

/*******************************************************************************
 * free_list:  a buffer to store uint64 values as bitmaps
 *                to store free elements of a buffer. the number of
 *                uint64s depends on the buffer's capacity.
 *             this is dynamically reallocated buffer
 *             Ex. for capacity<=64, 1x uint64 as free list,
 *                 for capacity>64 && <=128, 2x uint64 as free list, ...
 ******************************************************************************/
struct buffer_table_free_list {
    struct buffer_table_free_list* next;
};

static struct buffer_table_free_list* buffer_table_free_list_head = NULL;

static uint32 buffer_table_allocations_count = 0;
void
btable_flist_add(void* ptr)
{
    struct buffer_table_free_list* list = ptr;
    list->next = buffer_table_free_list_head;
    buffer_table_free_list_head = list;
    buffer_table_allocations_count++;
}

buffer_t*
btable_flist_get_free(void)
{
    if(!buffer_table) {
        buffer_arr_print_error(NO_BUFFER_ARR_TABLE_INIT);
    }
    struct buffer_table_free_list* list = buffer_table_free_list_head;
    buffer_table_free_list_head = buffer_table_free_list_head->next;
    buffer_table_count++;
    buffer_table_allocations_count--;
    return (buffer_t*) list;
}


void
buffer_arr_init_table(const pageid_t num_pages)
{
    if(buffer_table) {
        buffer_arr_print_error(BUFFER_ARR_TABLE_ALREADY_INIT);
        return;
    }

    allocator_instance_id = slab_allocator_create_new_instance(num_pages, "buffer_arr_init_table()");

    const uint32 btable_typesize = sizeof(buffer_t);
    assert(sizeof(buffer_t) > sizeof(void*));

    /* create memory for buffer table and buffer's table free list */
    void* address =
            (buffer_t*) aligned_alloc(BUFFER_ARR_ALIGNMENT, BUFFER_ARR_TABLE_MIN_CAPACITY * btable_typesize);

    buffer_table = address;

    buffer_t* btable_ptr = buffer_table;
    while((uint64) btable_ptr < (uint64) (buffer_table + BUFFER_ARR_TABLE_MIN_CAPACITY-1)) {
        btable_flist_add(btable_ptr);
        btable_ptr++;
    }


    /* DEBUG-FPRINT */
    fs_buffer = fs_create("Debug/buffer/buffer.txt");
    fs_register_created_buffers = fs_create("Debug/buffer/register_created_buffers.txt");
}

void
buffer_arr_destroy_table(void)
{
    if(buffer_table) {
        print("FREEING buffer_table: %x ... ", ry, buffer_table, CLEAR);
        free(buffer_table);
        print("OK\n\n", GREEN, CLEAR);
    }
}


/***************************************************************************************************
 * buffer constructors-destructors
 **************************************************************************************************/
buffer_t*
buffer_arr_constructor_uint32(const bufidx_t capacity)
{
    if(!buffer_table) {
        buffer_arr_print_error(NO_BUFFER_ARR_TABLE_INIT);
        my_exit("buffer_arr_constructor_uint32", __FILE__, __LINE__);
    }
    if(capacity <= 0) {
        buffer_arr_print_error(BUFFER_ARR_CAPACITY_LESS_THAN_1);
        my_exit("buffer_arr_constructor_uint32", __FILE__, __LINE__);
    }


    /* how many uint64's do we need to cover all buffer elements. 16 uint64's for buffer capacity=1024 */
    const uint32 free_list_uint64_count = ((capacity - 1) / BITS64) + 1;

    /* alloc storage for bufer data */
    const void* const data_ptr =
        slab_allocator_alloc(allocator_instance_id, capacity, sizeof(uint32), "buffer_arr_constructor_uint32()");
    /* alloc storage for buffer free-list data (a uint64 for each 64 buffer elements) */
    uint64* const buffer_arr_flist_ptr =
        slab_allocator_alloc(allocator_instance_id, free_list_uint64_count, sizeof(uint64), "buffer_arr_constructor_uint32()-free list");

    // file_sys_push(pname, sizeof(char*), TYPE_POINTER);

    buffer_t* buffer = btable_flist_get_free();
    buffer->free_list = buffer_arr_flist_ptr;
    buffer->free_list_capacity = free_list_uint64_count;
    buffer->data = (uint32*) data_ptr;
    buffer->capacity = capacity;
    buffer->count = 0;
    buffer->tail = INT_NULL;
    buffer->id = buffer_arr_get_id();
    buffer->typesize = sizeof(uint32);


    /* init buffer data */
    buffer_arr_memset(buffer, 0);

    /* init buffer free_list */
    buffer_arr_flist_memset(buffer, 0xffffffffffffffff);

    return buffer;
}


buffer_t*
buffer_arr_constructor_gentype(const bufidx_t capacity, const typesize_t typesize)
{
    // uint64 s = max_value_from_type(typesize);
    // assert(s < MAX_UINT32);

    if(!buffer_table) {
        // buffer_arr_init_table();
        buffer_arr_print_error(BUFFER_ARR_TABLE_INIT_FROM_CREATE_FUN);
        // my_exit("buffer_arr_constructor_uint32", __FILE__, __LINE__);
    }
    if(capacity <= 0) {
        buffer_arr_print_error(BUFFER_ARR_CAPACITY_LESS_THAN_1);
        my_exit("buffer_arr_constructor_uint32", __FILE__, __LINE__);
    }

    // how many uint64's do we need to cover all buffer elements. 16 uint64's for buffer capacity=1024
    const uint32 free_list_uint64_count = ((capacity - 1) / BITS64) + 1;

    /* allocate storage for bufer data */
    const void* const data_ptr = slab_allocator_alloc(allocator_instance_id, capacity, typesize, "buffer_arr_constructor_gentype()");

    /* allocate storage for buffer free-list data (an uint64 for each 64 buffer elements) */
    uint64* const buffer_arr_flist_ptr = slab_allocator_alloc(allocator_instance_id, free_list_uint64_count, sizeof(uint64), "buffer_arr_constructor_gentype()-free list");

    buffer_t* buffer = btable_flist_get_free();
    buffer->free_list = buffer_arr_flist_ptr;
    buffer->free_list_capacity = free_list_uint64_count;
    buffer->data = (void*) data_ptr;
    buffer->capacity = capacity;
    buffer->count = 0;
    buffer->tail = INT_NULL;
    buffer->id = buffer_arr_get_id();
    buffer->typesize = typesize;

    /* init buffer data */
    buffer_arr_memset(buffer, 0xcd);

    /* init buffer free_list */
    buffer_arr_flist_memset(buffer, 0xffffffffffffffff);


    /* DEBUG-FPRINT */
    fs_register_buffer_arr_create(fs_register_created_buffers, buffer);


    return buffer;
}

void
buffer_arr_destructor(buffer_t* buffer)
{
    void* const ptr = buffer;

    /* Release buffer's data slab */
    slab_allocator_release(allocator_instance_id, buffer->free_list, buffer->free_list_capacity, sizeof(uint64));
    slab_allocator_release(allocator_instance_id, buffer->data, buffer->capacity, buffer->typesize);

    /* DEBUG-FPRINT
     * write a string to buffer's->data memory
     * CAN COLOR MEMORY FOR SLAB SIZE > 8 BYTES */
    if(buffer->typesize > 8) {
        char_buf16_t dinfo;
        char_buf16_copy_from_str(&dinfo, "EMPTY");
        char inttostr[INT32_TO_STR_BUFFER_SIZE];
        str_conv_int32_to_str(inttostr, buffer->id);
        str_concat_memcpy(dinfo.buf, inttostr, 16);
        char* writable_data = buffer->data+sizeof(char*); // leave pointer size mem for slabs free list
        while(buffer->capacity) {

            memcpy((void*)writable_data, dinfo.buf, sizeof(char*));
            writable_data += buffer->typesize;
            buffer->capacity--;
            buffer->count--;
            buffer->tail--;
        }
    }

    /* DEBUG-FPRINT */

    buffer->data = NULL;
    buffer->free_list = NULL;
    buffer->free_list_capacity = 0;

    buffer->capacity = 0;
    buffer->typesize = 0;
    buffer->count = 0;
    buffer->tail = INT_NULL;


    /* free buffer from buffer table */
    btable_flist_add(ptr);
    buffer_table_count--;

}

void
buffer_arr_release(buffer_t* buffer)
{
    void* const ptr = buffer;

    /* Release buffer's data slab */
    slab_allocator_release(allocator_instance_id, buffer->data, buffer->capacity, buffer->typesize);


    /* DEBUG-FPRINT
     * write a string to buffer's->data memory
     */
    /* const char* dinfo = "DESTROYED ID:"; */
    char_buf16_t dinfo;
    char_buf16_copy_from_str(&dinfo, "DESTROYED ID:");
    char inttostr[INT32_TO_STR_BUFFER_SIZE];
    str_conv_int32_to_str(inttostr, buffer->id);
    str_concat_memcpy(dinfo.buf, inttostr, 16);
    memcpy(buffer->data, dinfo.buf, sizeof(char*)*2);

    buffer->data = NULL;
    buffer->free_list = NULL;
    buffer->free_list_capacity = 0;

    buffer->capacity = 0;
    buffer->typesize = 0;
    buffer->count = 0;
    buffer->tail = INT_NULL;


    /* free buffer from buffer table */
    btable_flist_add(ptr);
    buffer_table_count--; // ISSUES: move this to btable_flist_add BUT create an init btable function (so that counter is not decremened at init)

}


/*
 * helpers
 */
bufidx_t
buffer_arr_find_next_free_elem(buffer_t* const buffer)
{
    const uint64* free_list = buffer->free_list;

    /* run for all possible free_list uint64's */
    for(bufidx_t i = 0; i < buffer->free_list_capacity; i++) {
        /* check for none 0 uint64 (undefined behavior) */
        if(*(free_list + i) != 0) { // if  == 0 then there is no free element
            const int32 trailing_zeros = __builtin_ctzll(*(free_list + i));
            /* if (trailing_zeros != 0) */
            return trailing_zeros; // returns last none zero bit index
        }
    }

    // todo: if no last index is found ???
    my_exit("buffer_arr_find_next_free_elem: NO SPARSE_ARR FREE INDEX FOUND", __FILE__, __LINE__);
}


/***************************************************************************************************
 * PRIVATE
 **************************************************************************************************/
void
buffer_arr_reallocate(buffer_t* const buffer)
{
    /* 1. allocate new buffer
     * 2. copy data
     * 3, release old buffer data
     * 4. realloc buffers's free_list ptr
     * 5. copy free_list data
     * 6. release old buffer's free_list ptr */

    const bufidx_t new_capacity = buffer->capacity * 2;

    // 1.
    void* const data_ptr =
        (uint32*) slab_allocator_alloc(allocator_instance_id, new_capacity, buffer->typesize, "buffer_arr_reallocate()");

    // 2.
    memcpy(data_ptr, buffer->data, buffer->capacity * buffer->typesize);

    // 3.
    slab_allocator_release(allocator_instance_id, buffer->data, buffer->capacity, buffer->typesize);

    // 4.
    const uint32 flist_old_capacity = ((buffer->capacity - 1) / BITS64) + 1;
    const uint32 flist_capacity = ((new_capacity - 1) / BITS64) + 1;
    void* const free_list_ptr =
        (uint64*) slab_allocator_alloc(allocator_instance_id, flist_capacity, sizeof(uint64), "buffer_arr_reallocate()");
    memset(free_list_ptr, MAX_UINT32, flist_capacity * sizeof(uint64));

    // 5.
    memcpy(free_list_ptr, buffer->free_list, flist_old_capacity * sizeof(uint64));

    // 6.
    slab_allocator_release(allocator_instance_id, buffer->free_list, flist_old_capacity, sizeof(uint64));

    buffer->data = data_ptr;
    buffer->capacity = new_capacity;
    buffer->free_list = free_list_ptr;
    buffer->free_list_capacity = flist_capacity;
}

void
buffer_arr_memcopy(const buffer_t* const dst, const buffer_t* const src)
{
    memcpy(dst->data, src->data, src->capacity * sizeof(uint32));
}

void
buffer_arr_memset(const buffer_t* const buffer, const uint64 val)
{
    memset(buffer->data, val, buffer->capacity * buffer->typesize);
}

void
buffer_arr_flist_memset(const buffer_t* const buffer, const uint64 val)
{
    for(uint32 i = 0; i < buffer->free_list_capacity; i++) {
        buffer->free_list[i] = val;
    }
}

// todo: create generic type realloc()


/***************************************************************************************************
 * USELESS
 **************************************************************************************************/
bufidx_t
TEMP_buffer_arr_find_tail(const buffer_t* const buffer)
{
    // uint32 accum_num_bits = 0;
    // uint64 free_list = *buffer->free_list;
    // // todo: free_list is a buffer of uint64
    // const bufidx_t index = buffer_arr_find_tail_recur(free_list, &accum_num_bits);
    // print("accum_num_bits = %d\n", accum_num_bits);
    //
    // if(index == INT_NULL) {
    //     // no free index found
    //     // todo: realloc and/or implement searching to the second uint64 free buffer
    //     my_exit("index == INT_NULL", __FILE__, __LINE__);
    // }
    //
    // return index + accum_num_bits;
}

/* Find the last element in buffer
 * return: int32 index, -1 error  */
int32
buffer_arr_find_tail_recur(const uint64 val, uint32* accum_num_bits)
{
    // todo: realloc and/or implement searching to the second uint64 free buffer
    // todo: reimplement with: __builtin_ctzll();

    uint64 low = bits_mask_low(val);
    const uint32 type_size = types_get_bitsize_from_val(val);

    int32 index = INT_NULL;
    if(low != 0) {
        index = find_first_free_helper(low, type_size);
    } else {
        // on every recursion, cut type size by half, and accumulate all the previous devisions lower part
        *accum_num_bits += types_get_half_bitsize_from_type(low);
        const uint64 high = bits_mask_high(val);
        index = buffer_arr_find_tail_recur(high, accum_num_bits);
    }

    return index;
}


bufidx_t
find_first_free_helper(const uint32 val, const bufidx_t num_elems)
{
    for(bufidx_t i = 0; i < num_elems; i++) {
        const uint8 bit = bit_is_set(val, i, typeof(val));
        if(bit) { return i; }
    }
    return INT_NULL;
}



/* DEBUG-FPRINT */
void buffer_arr_print_to_file(buffer_t* buffer)
{
    fs_buffer_arr_append(fs_buffer,
        " count:%u |"
        " tail:%d |"
        " capacity:%u |"
        " data: %08x %08x "
        " addr: %08x %08x\n",
        buffer->count,
        buffer->tail,
        buffer->capacity,
        (uint64)buffer->data >> 32,(uint64)buffer->data & 0xFFFFFFFF,
        (uint64)&buffer >> 32,(uint64)&buffer & 0xFFFFFFFF
        );

    const uint32 elems_per_line = (buffer->capacity > 100) ? 10
    : (buffer->capacity < 10) ? 10 : buffer->capacity / 10;
    uint32* data = buffer->data;

    for(uint32 i = 0; i < buffer->tail; i++) {

        if((i+elems_per_line) % elems_per_line == 0) {
            fs_buffer_arr_append(fs_buffer, "\n");
        }

        // fs_buffer_arr_append(fs_buffer, "[%3u]= %5u, ", i, data[i]);
        fs_buffer_arr_append(fs_buffer, "%5u, ", data[i]);


    }
}