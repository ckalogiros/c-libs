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

#include "mem_cache.h"
#include "stringc.h"
#include "string_allocator_internals.h"
#include "string_mem_cache.h"


// todo: must be dynamicaly reallocated
#define BUFFER_TABLE_MIN_CAPACITY 512


/*******************************************************************************
 *
 ******************************************************************************/
static uint32 string_alloced_count = 0;


/***************************************************************************************************
 * FOR DEBUG */
/* output one debug file and register all created strings  */
static struct fs_t* fs_register_created_strings = NULL;
static struct fs_t* fs_register_realloced_strings = NULL;

#define DEBUG_FPRINT_BUFFER 1
#ifdef DEBUG_FPRINT_BUFFER
/* append info on string creation */
#define fs_register_string(fs, string, op)\
fs_append(fs,       \
"%10s: | "           \
"len:  %3u | "      \
"size: %3u | "      \
"self-size:  %3u | "\
"data: %08x %08x "  \
"data: %s\n",       \
op,           \
string->len,        \
string->size,       \
string->self_size,  \
(uint64)string->data >> 32,(uint64)string->data & 0xFFFFFFFF, \
string->data \
)


/* append formated data to the string's file */
#define fs_string_append(fs, fmt, ...)\
fs_append(fs, fmt, ##__VA_ARGS__)

#else

#endif
/* FOR DEBUG
 **************************************************************************************************/

static inline strlen_t string_get_self_size() { return sizeof(string_t) - sizeof(char); }


void
string_container_init(void)
{
    if(is_string_mcache_table_init()) return;

    string_mem_cache_table_init();

    /* DEBUG-FPRINT */
    fs_register_created_strings = fs_create("allocators/string/register_created_strings.txt");
    fs_register_realloced_strings = fs_create("allocators/string/register_realloced_strings.txt");
}

void
string_container_destroy(void)
{
    print("FREEING strings: calling string_mem_cache_table_destroy() from string container.");
    string_mem_cache_table_destroy();
}

/***************************************************************************************************
 * string constructors-destructors
 **************************************************************************************************/
string_t*
string_constructor(const char* str, const strlen_t size)
{
    const strlen_t len = strlen(str);
    const strlen_t self_size = string_get_self_size();

    string_size_t capacity = (size > STRING_MIN_LEN) ? size : len + self_size + 1;

    strlen_t max_size = STRING_MIN_LEN;
    while(max_size < capacity) max_size *= 2;


    // alloc storage for bufer data
    void* data_ptr = NULL;
    if(max_size <= 256)
        data_ptr = string_slab_alloc_allocate(max_size);
    else
        data_ptr = mem_cache_alloc_buffer(max_size, sizeof(char));

    string_t* string = data_ptr;
    string->len = 0;
    string->size = max_size;
    string->self_size = self_size;

    // init string data
    string_memset(string, 0x0);
    string_set_cstr(string, str);

    string_alloced_count++;

    /* DEBUG-FPRINT */
#ifdef DEBUG_FPRINT_MCACHE
    fs_register_string(fs_register_created_strings, string, "CREATE");
#endif

    return string;
}

void
string_destructor(string_t* string)
{
    // Release string's data slab
    if(string->size < 256)
    string_slab_alloc_release(string->data, string->size);
    else
    mem_cache_release_buffer(string->data, string->size, sizeof(char));

    /* DEBUG-FPRINT
     * write a string to string's->data memory
     * CAN COLOR MEMORY FOR SLAB SIZE > 8 BYTES */
    if(string->len > 8) {
        char_buf16_t dinfo;
        char_buf16_copy_from_str("EMPTY", &dinfo);
        // char inttostr[INT32_TO_STR_BUFFER_SIZE];
        // str_conv_int32_to_str(inttostr, string->id);
        // str_concat_memcpy(dinfo.buf, inttostr, 16);
        char* writable_data = string->data + sizeof(char*); // leave pointer size mem for slabs free list
        while(string->len) {
            memcpy((void*)writable_data, dinfo.buf, sizeof(char*));
            writable_data += sizeof(char);
            string->len--;
        }
    }

    /* DEBUG-FPRINT */

    string->data[0] = '\0';
    string->len = 0;

    // free string from string table
    string_alloced_count--;
}

void
string_release(string_t* string)
{
    // Release string's data slab
    if(string->size < 256)
        string_slab_alloc_release(string->data, string->size);
    else
        mem_cache_release_buffer(string->data, string->size, sizeof(char));

    /* DEBUG-FPRINT
     * write a string to string's->data memory
     */
    char_buf16_t dinfo;
    char_buf16_copy_from_str("DESTROYED ID:", &dinfo);
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
string_reallocate(string_t* string, const strlen_t len)
{
#ifdef DEBUG_FPRINT_MCACHE
    fs_register_string(fs_register_realloced_strings, string, "REALLOC");
#endif

    const strlen_t new_len = string->len + len + string->self_size+1;
    print("new len:%u\n", new_len);

    strlen_t max_size = STRING_MIN_LEN;
    while(max_size <= new_len) {
        assert(max_size > 0);
        max_size *= 2;
    }


    void* data_ptr = NULL;
    if(max_size <= 256)
        data_ptr = string_slab_alloc_allocate(max_size);
    else
        data_ptr = mem_cache_alloc_buffer(max_size, sizeof(char));


    strlen_t old_len = string->len;
    strlen_t old_self_size = string->self_size;
    memset((void*)data_ptr, 0, max_size);
    memcpy((void*)data_ptr, string, string->len+string->self_size);

    if(string->size < 256)
        string_slab_alloc_release(string->data, string->size);
    else
        mem_cache_release_buffer(string->data, string->size, sizeof(char));

    string = data_ptr;
    string->len = old_len;
    string->size = max_size;
    string->self_size = old_self_size;

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
