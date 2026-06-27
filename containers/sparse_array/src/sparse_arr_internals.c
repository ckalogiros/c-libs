/*
 * Created by ckalo on 1/11/26.
 */

#include "sparse_arr_internals.h"
#include "bits.h"
#include <stdlib.h>
#include <string.h>
#include "sparse_arr_log.h"
#include "logger.h"
#include "sys_utils.h"
#include "slab_allocator.h"
#include "file_sys_interface.h"
#include "slab_allocator_internals.h"
#include "str_utils.h"


/***************************************************************************************************
 * FOR DEBUG */
/* output specific buffers to file */
static struct fs_t* fs_buffer = NULL;
/* output one debug file and register all created buffers  */
static struct fs_t* fs_register_created_buffers = NULL;
// todo: must be dynamicaly reallocated

#define SPARSE_ARR_TABLE_MIN_CAPACITY 512
#define SPARSE_ARR_ALIGNMENT 1024


#define DEBUG_FPRINT_BUFFER 1
#ifdef DEBUG_FPRINT_BUFFER
/* append info on buffer creation */
#define fs_register_sparse_arr_create(fs, buffer)\
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
#define fs_sparse_arr_append(fs, fmt, ...)\
fs_append(fs, fmt, ##__VA_ARGS__)

#else

#endif
/* FOR DEBUG
 **************************************************************************************************/


/*******************************************************************************
 * sparse_arr_table: a buffer to store all applications buffer structures
 ******************************************************************************/
/* an array of pointers of type sparse_arr_t, that the [0] elem is the pointer to memory block start */
sparse_arr_t* sparse_arr_table = NULL;
uint32 sparse_arr_table_count = 0;
uint32 ID = 0;
uint16 allocator_instance_id = 0;
uint16 sparse_arr_get_allocator_instance_id(){ return allocator_instance_id; }


uint32 sparse_arr_get_id(){ return ++ID; }



void
TEMP_PRINT_SPARSE_ARR_TABLE_COUNT()
{
    print("TEMP_PRINT_SPARSE_ARR_TABLE_COUNT:%u\n", sparse_arr_table_count);
}

/*******************************************************************************
 * free_list:  a buffer to store uint64 values as bitmaps
 *                to store free elements of a buffer. the number of
 *                uint64s deppends on the buffer's capacity.
 *             this is dynamicaly reallocated buffer
 *             Ex. for capacity<=64, 1x uint64 as free list,
 *                 for capacity>64 && <=128, 2x uint64 as free list, ...
 ******************************************************************************/
struct sparse_arr_table_free_list {
    struct sparse_arr_table_free_list* next;
};

static struct sparse_arr_table_free_list* sparse_arr_table_free_list_head = NULL;

static uint32 sparse_arr_table_allocations_count = 0;
void
btable_flist_add(void* ptr)
{
    struct sparse_arr_table_free_list* list = ptr;
    list->next = sparse_arr_table_free_list_head;
    sparse_arr_table_free_list_head = list;
    sparse_arr_table_allocations_count++;
}

sparse_arr_t*
btable_flist_get_free(void)
{
    if(!sparse_arr_table) {
        sparse_arr_print_error(NO_SPARSE_ARR_TABLE_INIT);
    }
    struct sparse_arr_table_free_list* list = sparse_arr_table_free_list_head;
    sparse_arr_table_free_list_head = sparse_arr_table_free_list_head->next;
    sparse_arr_table_count++;
    sparse_arr_table_allocations_count--;
    return (sparse_arr_t*) list;
}


void
sparse_arr_init_table(const pageid_t num_pages)
{
    if(sparse_arr_table) {
        sparse_arr_print_error(SPARSE_ARR_TABLE_ALREADY_INIT);
        return;
    }

    allocator_instance_id = slab_allocator_create_new_instance(num_pages, "sparse_arr_init_table()");

    const uint32 btable_typesize = sizeof(sparse_arr_t);
    assert(sizeof(sparse_arr_t) > sizeof(void*));

    /* create memory for buffer table and buffer's free list */
    void* address =
            (sparse_arr_t*) aligned_alloc(SPARSE_ARR_ALIGNMENT, SPARSE_ARR_TABLE_MIN_CAPACITY * btable_typesize);

    sparse_arr_table = address;

    sparse_arr_t* btable_ptr = sparse_arr_table;
    while((uint64) btable_ptr < (uint64) (sparse_arr_table + SPARSE_ARR_TABLE_MIN_CAPACITY-1)) {
        btable_flist_add(btable_ptr);
        btable_ptr++;
    }


    /* DEBUG-FPRINT */
    fs_buffer = fs_create("Debug/buffer/buffer.txt");
    fs_register_created_buffers = fs_create("Debug/buffer/register_created_buffers.txt");
}

void
sparse_arr_destroy_table(void)
{
    if(sparse_arr_table) {
        print("FREEING sparse_arr_table: %x ... ", ry, sparse_arr_table, CLEAR);
        free(sparse_arr_table);
        print("OK\n\n", GREEN, CLEAR);
    }
}


/***************************************************************************************************
 * buffer constructors-destructors
 **************************************************************************************************/
sparse_arr_t*
sparse_arr_constructor_uint32(const bufidx_t capacity)
{
    if(!sparse_arr_table) {
        sparse_arr_print_error(NO_SPARSE_ARR_TABLE_INIT);
        my_exit("sparse_arr_constructor_uint32", __FILE__, __LINE__);
    }
    if(capacity <= 0) {
        sparse_arr_print_error(SPARSE_ARR_CAPACITY_LESS_THAN_1);
        my_exit("sparse_arr_constructor_uint32", __FILE__, __LINE__);
    }


    /* how many uint64's do we need to cover all buffer elements. 16 uint64's for buffer capacity=1024 */
    const uint32 free_list_uint64_count = ((capacity - 1) / BITS64) + 1;

    /* alloc storage for bufer data */
    const void* const data_ptr =
        slab_allocator_alloc(allocator_instance_id, capacity, sizeof(uint32), "sparse_arr_constructor_uint32()");
    /* alloc storage for buffer free-list data (a uint64 for each 64 buffer elements) */
    uint64* const sparse_arr_flist_ptr =
        slab_allocator_alloc(allocator_instance_id, free_list_uint64_count, sizeof(uint64), "sparse_arr_constructor_uint32()-free list");

    // file_sys_push(pname, sizeof(char*), TYPE_POINTER);

    sparse_arr_t* buffer = btable_flist_get_free();
    buffer->free_list = sparse_arr_flist_ptr;
    buffer->free_list_capacity = free_list_uint64_count;
    buffer->data = (uint32*) data_ptr;
    buffer->capacity = capacity;
    buffer->count = 0;
    buffer->tail = INT_NULL;
    buffer->id = sparse_arr_get_id();
    buffer->typesize = sizeof(uint32);


    /* init buffer data */
    sparse_arr_memset(buffer, 0);

    /* init buffer free_list */
    sparse_arr_flist_memset(buffer, 0xffffffffffffffff);

    return buffer;
}

sparse_arr_t*
sparse_arr_constructor_gentype(const bufidx_t capacity, const typesize_t typesize)
{
    if(!sparse_arr_table) {
        // sparse_arr_init_table();
        sparse_arr_print_error(SPARSE_ARR_TABLE_INIT_FROM_CREATE_FUN);
        // my_exit("sparse_arr_constructor_uint32", __FILE__, __LINE__);
    }
    if(capacity <= 0) {
        sparse_arr_print_error(SPARSE_ARR_CAPACITY_LESS_THAN_1);
        my_exit("sparse_arr_constructor_uint32", __FILE__, __LINE__);
    }

    // how many uint64's do we need to cover all buffer elements. 16 uint64's for buffer capacity=1024
    const uint32 free_list_uint64_count = ((capacity - 1) / BITS64) + 1;

    /* allocate storage for bufer data */
    const void* const data_ptr = slab_allocator_alloc(allocator_instance_id, capacity, typesize, "sparse_arr_constructor_gentype()");

    /* allocate storage for buffer free-list data (an uint64 for each 64 buffer elements) */
    uint64* const sparse_arr_flist_ptr = slab_allocator_alloc(allocator_instance_id, free_list_uint64_count, sizeof(uint64), "sparse_arr_constructor_gentype()-free list");

    sparse_arr_t* buffer = btable_flist_get_free();
    buffer->free_list = sparse_arr_flist_ptr;
    buffer->free_list_capacity = free_list_uint64_count;
    buffer->data = (void*) data_ptr;
    buffer->capacity = capacity;
    buffer->count = 0;
    buffer->tail = INT_NULL;
    buffer->id = sparse_arr_get_id();
    buffer->typesize = typesize;

    /* init buffer data */
    sparse_arr_memset(buffer, 0xcd);

    /* init buffer free_list */
    sparse_arr_flist_memset(buffer, 0xffffffffffffffff);


    /* DEBUG-FPRINT */
    fs_register_sparse_arr_create(fs_register_created_buffers, buffer);


    return buffer;
}

void
sparse_arr_destructor(sparse_arr_t* buffer)
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
    sparse_arr_table_count--;

}

void
sparse_arr_release(sparse_arr_t* buffer)
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
    sparse_arr_table_count--; // TODO: move this to btable_flist_add BUT create an init btable function (so that counter is not decremened at init)

}


/*
 * helpers
 */
bufidx_t
sparse_arr_find_next_free_elem(sparse_arr_t* const buffer)
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
    my_exit("sparse_arr_find_next_free_elem: NO SPARSE_ARR FREE INDEX FOUND", __FILE__, __LINE__);
}


/***************************************************************************************************
 * PRIVATE
 **************************************************************************************************/
void
sparse_arr_reallocate(sparse_arr_t* const buffer)
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
        (uint32*) slab_allocator_alloc(allocator_instance_id, new_capacity, buffer->typesize, "sparse_arr_reallocate()");

    // 2.
    memcpy(data_ptr, buffer->data, buffer->capacity * buffer->typesize);

    // 3.
    slab_allocator_release(allocator_instance_id, buffer->data, buffer->capacity, buffer->typesize);

    // 4.
    const uint32 flist_old_capacity = ((buffer->capacity - 1) / BITS64) + 1;
    const uint32 flist_capacity = ((new_capacity - 1) / BITS64) + 1;
    void* const free_list_ptr =
        (uint64*) slab_allocator_alloc(allocator_instance_id, flist_capacity, sizeof(uint64), "sparse_arr_reallocate()");
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
sparse_arr_memcopy(const sparse_arr_t* const dst, const sparse_arr_t* const src)
{
    memcpy(dst->data, src->data, src->capacity * sizeof(uint32));
}

void
sparse_arr_memset(const sparse_arr_t* const buffer, const uint64 val)
{
    memset(buffer->data, val, buffer->capacity * buffer->typesize);
}

void
sparse_arr_flist_memset(const sparse_arr_t* const buffer, const uint64 val)
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
TEMP_sparse_arr_find_tail(const sparse_arr_t* const buffer)
{
    // uint32 accum_num_bits = 0;
    // uint64 free_list = *buffer->free_list;
    // // todo: free_list is a buffer of uint64
    // const bufidx_t index = sparse_arr_find_tail_recur(free_list, &accum_num_bits);
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
sparse_arr_find_tail_recur(const uint64 val, uint32* accum_num_bits)
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
        index = sparse_arr_find_tail_recur(high, accum_num_bits);
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
void sparse_arr_print_to_file(sparse_arr_t* buffer)
{
    fs_sparse_arr_append(fs_buffer,
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
            fs_sparse_arr_append(fs_buffer, "\n");
        }

        // fs_sparse_arr_append(fs_buffer, "[%3u]= %5u, ", i, data[i]);
        fs_sparse_arr_append(fs_buffer, "%5u, ", data[i]);


    }
}