/*
 * Created by ckalo on 1/4/26.
 */

#include "mem_man_internals.h"
#include "mem_log.h"
#include "sys_utils.h"
#include "internal/mem_structs.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include "mem_test.h"


#define CHECK_MEM_NULL(p, error_code, msg) if(p==NULL){\
    mem_print_error(error_code);\
    my_exit(msg, __FILE__, __LINE__);\
}

#define CHECK_CONTIGUOUS_FREE_MEM(p1, offset, p2, error_code, msg)\
if((uint64*)((uint8*)p1+offset) < (uint64*)p2){\
    mem_print_error(error_code);\
    print("free list: %x\nnext flist:%x\n", REDUCE_ADDR(p1, F6), REDUCE_ADDR(p2, F6));\
    my_exit(msg, __FILE__, __LINE__);\
}

/***************************************************************************************************
 * memory manager -> mms
 * is a single allocation at the start of a program, no realloc is implemented,
 * so is plug&play to find the appropriate total mem_chunk size a program needs.
 **************************************************************************************************/

static struct mem* mem;
static struct mem_chunk* chunks_table;       // mem is devided into chunks
// static struct mem_used* mem_used_table;      // table stores: mem used buffers per chunk
static struct free_list** flist_heads_table; // table stores:flist heads per chunk

struct mem* mem_get(){}
// struct mem_chunk* mem_chunk_get(const chunkid_t id){ return chunks_table[id]; }
// struct mem_used* mem_used_get(const pageid_t id){ return mem_used_table[id]; }


#define flist_table_get_next_free(chunk_id)\
    (flist_heads_table[chunk_id] = flist_heads_table[chunk_id]->next)
struct free_list* flist_table_get_head(const chunkid_t id){ return flist_heads_table[id]; }




void mem_init_chunks_table();
void mem_init_used_mem_table();
void mem_init_free_list_table();


bool
mem_init()
{
    if(mem) return FALSE;

    const uint32 page_size = PAGE_SIZE;
    const uint32 num_pages = 512;
    const uint32 alignment = NUM_CHUNKS*PAGE_SIZE;
    const uint32 self_storage_size  = PAGE_SIZE;
    const uint32 memory_bound_check = PAGE_SIZE;

    const uint32 total_size = self_storage_size + (num_pages * page_size) + memory_bound_check;

    void* addr = aligned_alloc(alignment, total_size);
    print("addr:%p\n", addr);
    print("end addr:%p\n", addr + total_size);

    mem = addr;
    mem->start = addr;
    mem->size = total_size;
    mem->chunks_count = 0;

    mem->alloc_start = addr + self_storage_size;

    mem_init_chunks_table();

    mem_init_used_mem_table();

    mem_init_free_list_table();


    /* *************************************************
     * set off boundary area, for corruption detection */
    void* off_bound_mem_start = addr+(self_storage_size + (num_pages * page_size));
    uint64* ptr = (uint64*) off_bound_mem_start;
    const uint32 len = PAGE_SIZE / sizeof(uint64);
    for(uint32 i = 0; i < len; i++) {
        ptr[i] = MEM_OFF_BOUNDARIES_CHECK;
    }
    // test it
    bool memory_corrupted = check_mem_off_boundaries_writes(off_bound_mem_start, PAGE_SIZE);
    if (memory_corrupted) {
        my_exit("ERROR: MEMORY CORRUPTION DETECTED", __FILE__, __LINE__);
    }

    return TRUE;

}

void mem_init_chunks_table()
{
    void* ptr = GET_ADDR_OF_MEM(mem);
    // advance pointer to free memory, where we can store chunks_table
    ptr = ptr + sizeof(struct mem);

    chunks_table = ptr; // start of struct chunks_table storage

    /*************************************************************
     * chunks_table storage layout
     *************************************************************/
    for(uint32 i = 0; i < NUM_CHUNKS; i++) {
        chunks_table[i].id = i;
        chunks_table[i].flags.is_init = 0;

        // properly count total number of chunks
        mem->chunks_count++;
    }

    // mem_test();

}

static memid_t mem_get_chunkid_by_ids(const memids_t ids)
{
    memid_t val = ids;
    memid_t start_bit = flag_ids.chunk_id_start_bit;
    memid_t end_bit = flag_ids.chunk_id_end_bit;
    uint32 mask = bits_calc_bitmask_from_bits(val, start_bit, end_bit);
    const memid_t chunk_id = bitfield_get(val, mask, start_bit);

    return chunk_id;

}
static memid_t mem_get_pageid_by_ids(const memids_t ids)
{
    memid_t val = ids;
    memid_t start_bit = flag_ids.page_id_start_bit;
    memid_t end_bit = flag_ids.page_id_end_bit;
    uint32 mask = bits_calc_bitmask_from_bits(val, start_bit, end_bit);
    const memid_t page_id = bitfield_get(val, mask, start_bit);

    return page_id;

}
static bool mem_used_set_chunkid(const pageid_t page_id, const memid_t id)
{

    /* *****************************************
     * calc mask and set bitfield of chunks id */
    memid_t val       = mem_used_table[page_id].ids;
    memid_t sval      = id; // id is the index (as offset) of a chunk in the table
    memid_t start_bit = flag_ids.chunk_id_start_bit;
    memid_t end_bit   = flag_ids.chunk_id_end_bit;

    uint32 chunk_id_mask = bits_calc_bitmask_from_bits(val, start_bit, end_bit);
    mem_used_table[page_id].ids = bitfield_set(val, sval, chunk_id_mask, start_bit);
    return TRUE;
}
static bool mem_used_set_pageid(const pageid_t page_id, const memid_t id)
{
    /* *****************************************
     * calc mask and set bitfield of page id */
    memid_t val       = mem_used_table[page_id].ids;
    memid_t sval      = id;  // id is the index (as offset) of a page in the table
    memid_t start_bit = flag_ids.page_id_start_bit;
    memid_t end_bit   = flag_ids.page_id_end_bit;

    uint32 page_id_mask = bits_calc_bitmask_from_bits(val, start_bit, end_bit);
    mem_used_table[page_id].ids = bitfield_set(val, sval, page_id_mask, start_bit);

    return TRUE;
}

void mem_init_used_mem_table()
{
    /*************************************************************
     * mem_used_table storage layout
     *************************************************************/

    void* ptr = GET_ADDR_OF_MEM(mem);
    // advance pointer to free memory, where we can store mem_used_table
    ptr = ptr + sizeof(struct mem); // advance through struct mem
    // advance through struct mem_chunk * number of chunks
    ptr = ptr + ( sizeof(struct mem_chunk) * mem->chunks_count);

    mem_used_table = ptr; // start of struct mem_used_table storage


    for(uint32 chunk_id = 0; chunk_id < mem->chunks_count; chunk_id++) {
        for(uint32 page_id = 0; page_id < PAGES_PER_CHUNK; page_id++) {

            /* ****************************************
             * passing page_id as the index to current
             * used_mem in table and set the chunk id */
            mem_used_set_chunkid((pageid_t) page_id, chunk_id);

            mem_used_set_pageid((pageid_t) page_id, page_id);

            // print-debug bitfield chunk_ids and page_ids
            print("[%u] page_id:%u ", page_id, mem_get_pageid_by_ids(mem_used_table[page_id].ids));
            print("chunk_id:%u\n", mem_get_chunkid_by_ids(mem_used_table[page_id].ids));
        }
    }
}

void mem_init_free_list_table()
{
    /*************************************************************
     * flist_heads_table storage layout
     *************************************************************/

    void* ptr = GET_ADDR_OF_MEM(mem);
    // advance pointer to free memory, where we can store mem_used_table
    ptr = ptr + sizeof(struct mem); // advance through struct mem
    // advance through struct mem_chunk * number of chunks
    ptr = ptr + ( sizeof(struct mem_chunk) * mem->chunks_count);

    ptr = ptr + ( sizeof(struct mem_used) * PAGES_PER_CHUNK);

    flist_heads_table = ptr; // start of struct free_list storage

    // initialize all free list heads and free lists for each mem_chunk
    for(uint32 chunk_id = 0; chunk_id < mem->chunks_count; chunk_id++) {

        // initialize all free list heads
        // flist_heads_table[chunk_id] = (struct free_list*) ptr;
        flist_heads_table[chunk_id] = (struct free_list*) ptr;
        flist_heads_table[chunk_id]->next = NULL;

        // advance pointer
        ptr += sizeof(struct free_list);

    }


    void* page_ptr = mem->alloc_start;

    for(uint32 chunk_id = 0; chunk_id < mem->chunks_count; chunk_id++) {

        /* ***********************************************************
         * start to register all pages of a chunk to the free list
         * the free list is build forward, that means the last page
         * of a chunk is pointed by the free list head of that chunk */

        // create storage for head pointer
        flist_heads_table[chunk_id] = (struct free_list*) page_ptr;

        // assign to each fflist head the mem start of each chunk
        struct free_list* flist_head = flist_heads_table[chunk_id];
        flist_head->next = NULL;
        // flist_heads_table[chunk_id] = flist_head;
        // print("%sflist head:%x%s\n", YELLOW, REDUCE_ADDR(flist_head, F5), CLEAR);

        for(uint16 i = 0; i < PAGES_PER_CHUNK; i++) {

            /* ***********************************************************
             * create new free list, for every page of a chunk, via the page pointer,
             * connect ->next to point at previous page hold by head,
             * finally set head to current page
             *  */
            // store the page id in each free page, not using any storage variable, just magic!
            // memid_t* page_id = (memid_t*)(page_ptr + sizeof(struct free_list*));
            // *page_id = i;
            *(memid_t*)(((struct free_list*)page_ptr) + 1) = mem_used_table[i].ids;


            page_ptr += PAGE_SIZE;

            struct free_list* flist = (struct free_list*) page_ptr;
            flist->next = flist_head;
            flist_head = flist;

            // print("flist:%x\n", REDUCE_ADDR(flist, F5));
        }
    }

}

void mem_test()
{
    const char* test_name = "Check if MACRO GET_ADDR_OF_CHUNK_BY_ID()\n calculates correct chunk address";
    print("### mem_test:\n %s\n### START\n", test_name);

    for(uint32 chunk_id = 0; chunk_id < NUM_CHUNKS; chunk_id++) {
        void* chunk_address = (char*) mem + sizeof(struct mem) + (sizeof(struct mem_chunk) * chunk_id);
        print("chunk[%u]:%p\n", chunk_id, chunk_address);
        print("chunk[%u]:%p\n", chunk_id, GET_ADDR_OF_CHUNK_BY_ID(mem, chunk_id));
        if(chunk_address == GET_ADDR_OF_CHUNK_BY_ID(mem, chunk_id))
            print("GET_ADDR_OF_CHUNK_BY_ID: %sPASSED%s\n", GREEN, CLEAR);
        else
            print("GET_ADDR_OF_CHUNK_BY_ID: %sFAILED%s\n", RED, CLEAR);
        print("\n");
    }
    print("### mem_test:\n %s\n### END\n\n", test_name);
}



// TODO: move to bitfields project(when project is created)
void test_calc_bitmask_from_start_end_bits()
{

    {
        print("-- TEST 1 \n   \'test_calc_bitmask_from_start_end_bits MACRO impl\'\n");
        const uint32 nbits = 6;
        const uint32 start_bit = 0;
        const uint32 end_bit = start_bit + nbits;
        uint32 mask = bits_calc_bitmask_from_bits(start_bit, start_bit, end_bit);

        print("%sresult mask%s\n", ry, CLEAR);
        bits_print_binary(mask, sizeof(mask));

        const uint32 expected = 0b111111;
        print("%sexpected mask%s\n", ry, CLEAR);
        bits_print_binary(expected, sizeof(expected));

        if(mask == expected) {
            print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
        }
        else
            print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);

        print("\n");
    }
    {
        print("-- TEST 1 \n   \'test_calc_bitmask_from_start_end_bits MACRO impl\'\n");
        const uint32 nbits = 6;
        const uint32 start_bit = 4;
        const uint32 end_bit = start_bit + nbits;
        uint32 mask = bits_calc_bitmask_from_bits(start_bit, start_bit, end_bit);

        print("%sresult mask%s\n", ry, CLEAR);
        bits_print_binary(mask, sizeof(mask));

        const uint32 expected = 0b1111110000;
        print("%sexpected mask%s\n", ry, CLEAR);
        bits_print_binary(expected, sizeof(expected));

        if(mask == expected) {
            print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
        }
        else
            print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);

        print("\n");
    }
    {
        print("-- TEST 1 \n   \'test_calc_bitmask_from_start_end_bits MACRO impl\'\n");
        const uint32 nbits = 17;
        uint32 start_bit = 11;
        const uint32 end_bit = start_bit + nbits;
        const uint32 mask = bits_calc_bitmask_from_bits(start_bit, start_bit, end_bit);

        print("%sresult mask%s\n", ry, CLEAR);
        bits_print_binary(mask, sizeof(mask));

        const uint32 expected = 0b00001111111111111111100000000000;
        print("%sexpected mask%s\n", ry, CLEAR);
        bits_print_binary(expected, sizeof(expected));

        if(mask == expected) {
            print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
        }
        else
            print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);

        print("\n");
    }
}

void test_bitfield_set()
{

    {
        print("-- TEST 1 \n   \'bitfield_set function impl\'\n");
        const uint16 val      = 0b111111110000;
        const uint16 sval     = 0b1010;
        const uint16 mask     = 0b11110000;
        const uint16 nbits    = 4;

        print("val\n");
        bits_print_binary(val, sizeof(val));
        print("sval\n");
        bits_print_binary(sval, sizeof(sval));
        print("mask\n");
        bits_print_binary(mask, sizeof(mask));
        print("nbits:%u\n", nbits);

        uint16 val_masked_inv = val & (~mask);
        uint16 sval_shifted_masked = (sval << nbits) & mask;
        uint16 res = val_masked_inv | sval_shifted_masked;

        print("val_masked_inv\n");
        bits_print_binary(val_masked_inv, sizeof(val_masked_inv));
        print("sval_shifted_masked\n");
        bits_print_binary(sval_shifted_masked, sizeof(sval_shifted_masked));
        print("%sresult%s\n", ry, CLEAR);
        bits_print_binary(res, sizeof(res));

        uint16 expected = 0b111110100000;
        print("%sexpected result%s\n", ry, CLEAR);
        bits_print_binary(expected, sizeof(expected));

        if(res == expected) {
            print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
        }
        else
            print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);

        print("\n");
    }
    {
        print("-- TEST 2 \n   \'bitfield_set function impl\'\n");
        const uint16 val      = 0b100011110000;
        const uint16 sval     = 0b1010;
        const uint16 mask     = 0b11110000;
        const uint16 nbits    = 4;

        print("val\n");
        bits_print_binary(val, sizeof(val));
        print("sval\n");
        bits_print_binary(sval, sizeof(sval));
        print("mask\n");
        bits_print_binary(mask, sizeof(mask));
        print("nbits:%u\n", nbits);

        uint16 val_masked_inv = val & (~mask);
        uint16 sval_shifted_masked = (sval << nbits) & mask;
        uint16 res = val_masked_inv | sval_shifted_masked;

        print("val_masked_inv\n");
        bits_print_binary(val_masked_inv, sizeof(val_masked_inv));
        print("sval_shifted_masked\n");
        bits_print_binary(sval_shifted_masked, sizeof(sval_shifted_masked));
        print("%sresult%s\n", ry, CLEAR);
        bits_print_binary(res, sizeof(res));

        const uint16 expected = 0b0000100010100000;
        print("%sexpected result%s\n", ry, CLEAR);
        bits_print_binary(expected, sizeof(expected));

        if(res == expected) {
            print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
        }
        else
            print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);

        print("\n");
    }
}

void test_macro_impl_bitfield_set()
{

    {
        print("-- TEST 1 \n   \'bitfield_set MACRO impl\'\n");
        const uint16 val      = 0b111111110000;
        const uint16 sval     = 0b1010;
        const uint16 start_bit= 4;
        const uint16 end_bit  = 8;
        const uint16 nbits    = 4;
        uint16 mask     = 0;

        // expected_mask = 0b11110000;
        mask = bits_calc_bitmask_from_bits(mask, start_bit, end_bit);

        print("val\n");
        bits_print_binary(val, sizeof(val));
        print("sval\n");
        bits_print_binary(sval, sizeof(sval));
        print("mask\n");
        bits_print_binary(mask, sizeof(mask));
        print("nbits:%u\n", nbits);

        uint16 res = bitfield_set(val, sval, mask, start_bit);

        print("%sresult%s\n", ry, CLEAR);
        bits_print_binary(res, sizeof(res));

        uint16 expected = 0b111110100000;
        print("%sexpected result%s\n", ry, CLEAR);
        bits_print_binary(expected, sizeof(expected));

        if(res == expected) {
            print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
        }
        else
            print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);

        print("\n");
    }
    {
        print("-- TEST 2 \n   \'bitfield_set MACRO impl\'\n");
        uint16 val      = 0b100011110000;
        uint16 sval     = 0b1010;
        uint16 mask     = 0;
        uint16 start_bit= 4;
        uint16 end_bit  = 8;
        uint16 nbits    = 4;

        // expected_mask =  0b11110000;
        mask = bits_calc_bitmask_from_bits(mask, start_bit, end_bit);

        print("val\n");
        bits_print_binary(val, sizeof(val));
        print("sval\n");
        bits_print_binary(sval, sizeof(sval));
        print("mask\n");
        bits_print_binary(mask, sizeof(mask));
        print("nbits:%u\n", nbits);

        uint16 res = bitfield_set(val, sval, mask, start_bit);

        print("%sresult%s\n", ry, CLEAR);
        bits_print_binary(res, sizeof(res));

        uint16 expected = 0b0000100010100000;
        print("%sexpected result%s\n", ry, CLEAR);
        bits_print_binary(expected, sizeof(expected));

        if(res == expected) {
            print("bitfield_set(): %sPASSED%s\n", GREEN, CLEAR);
        }
        else
            print("bitfield_set(): %sFAILED%s\n", RED, CLEAR);

        print("\n");
    }
}

void temp_print_mem_structs_sizes()
{
    print(
        "mem size:%u\n"
        "mem_chunk size:%u\n"
        "mem_used size:%u\n",
        sizeof(struct mem),
        sizeof(struct mem_chunk),
        sizeof(struct mem_used)
    );
}

// new code
/**************************************************************************************************/

// private-helpers todo: move to another file?

memid_t chunk_selector_get_chunk(enum MEM_CHUNK_SELECTOR chunk_selector)
{
    if(chunk_selector == MEM_CHUNK_ALLOCATOR_SLAB) return MEM_CHUNK_ALLOCATOR_SLAB;

    return MEM_CHUNK_ANY;
}

/**************************************************************************************************/



struct mem_block mem_block = {.addr = NULL, .size = 0, .num_pages = 0};

void*
mem_get_mem() { return &mem_block; }

void*
mem_get_address() { return mem_block.addr; }

uint64
mem_get_total_size() { return mem_block.size; }

uint32
mem_get_num_pages() { return mem_block.num_pages; }


/***************************************************************************************************
 * free list
 * adds a free list node per PAGE, storing the free list node
 * in the memory PAGE block.
 **************************************************************************************************/
free_list_t* free_list_head = NULL;

const free_list_t* const free_list_get_head() { return free_list_head; }


void*
flist_get_next_free_page(pageid_t num_pages, const memid_t chunk_id)
{


    struct free_list* flist_head = flist_table_get_head(chunk_id);

    if(num_pages == 1) {
        return (void*)flist_head;
    }

    struct free_list* free = flist_head;
    struct free_list* prev = flist_head;
    struct free_list* cur = flist_head->next;
    num_pages--;
    while(num_pages--) {


        CHECK_CONTIGUOUS_FREE_MEM(prev, PAGE_SIZE, free_list_head,
                                  ALLOC_NOT_CONTIGUOUS_MEM, "flist_get_next_free_page");

        prev = cur;
        cur = cur->next;

        CHECK_MEM_NULL(cur, NO_FREE_MEMORY, "flist_get_next_free_page");

    }
    // mem_log_print_list_node("remove", free);
    return free;
}

pageid_t flist_get_pageid_magically(struct free_list* fnode)
{ // todo: make sure the free list node is actually free
    // a pageid exists right after the free list node
    const memids_t ids = *((pageid_t*)fnode +  1);
    const pageid_t pageid = mem_get_pageid_by_ids(ids);
    return pageid;
}

bool
free_list_insert(const pageid_t id, pageid_t num_pages)
{
    // get the address based on the page index id
    void* addr = mem_block.addr + (id * PAGE_SIZE);

    // case there is no free memory
    if(free_list_head == NULL) {
        free_list_head = (free_list_t*) addr;
        free_list_head->addr = addr;
        free_list_head->page_id = (addr - mem_block.addr) / PAGE_SIZE;


        free_list_t* fnode = free_list_head;
        num_pages--;

        while(num_pages > 0) {
            addr = (void*) (((uint8*) addr) + PAGE_SIZE); // increment to next page
            fnode->next = (free_list_t*) addr;
            fnode->next->page_id = (addr - mem_block.addr) / PAGE_SIZE;
            fnode->next->addr = addr;

            fnode = fnode->next;
            num_pages--;
        }

        fnode->next = NULL;
        // mem_log_print_list_node("insert", free_list_head);
        return TRUE;
    }

    // case freed addr is before the free list head
    if(((int64) addr - (int64) free_list_head) < 0) {
        free_list_t* temp = free_list_head; // so we don't loose ->next

        free_list_head = (free_list_t*) addr;
        free_list_head->addr = addr;
        free_list_head->page_id = (addr - mem_block.addr) / PAGE_SIZE;

        free_list_t* fnode = free_list_head;
        num_pages--;

        while(num_pages > 0) {
            addr = (void*) (((uint8*) addr) + PAGE_SIZE); // increment to next page
            fnode->next = (free_list_t*) addr;
            fnode->next->page_id = (addr - mem_block.addr) / PAGE_SIZE;
            fnode->next->addr = addr;

            fnode = fnode->next;
            num_pages--;
        }

        fnode->next = temp;
        return TRUE;
    }

    // else: walk through the list, find the free elem just before the address
    free_list_t* fnode = free_list_head;
    while(fnode != NULL) {
        // if(fnode->next && fnode->next > addr){
        if(fnode->next == NULL || (uint64*) fnode->next > (uint64*) addr) {
            free_list_t* temp = fnode->next; // so we don't loose ->next
            while(num_pages > 0) {
                fnode->next = (free_list_t*) addr;
                fnode->next->page_id = (addr - mem_block.addr) / PAGE_SIZE;
                fnode->next->addr = addr;

                fnode = fnode->next;
                num_pages--;
                addr = (void*) (((uint8*) addr) + PAGE_SIZE); // increment to next page
            }

            fnode->next = temp;
            return TRUE;
        }

        fnode = fnode->next;
    }

    return FALSE;
}



/***************************************************************************************************
 * used memory
 * is a buffer registering only calls to mms_alloc().
 * That means each element in used_mem buffer may be 1-many PAGES,
 * compare to free_list that registers a free element per PAGE.
 * On free used memory, one element of used_mem is "release"
 * where as for the free_list we run a loop to set free each
 * individual page.
 **************************************************************************************************/
used_m used_mem[START_USED_COUNT];
uint32 used_mem_count = 0; // number of used elements
int32 used_mem_tail = INT_NULL; // index of the last element in use. Ex. [1, 0,
// 0, 1, 0, 0]: tail should be at index 3

used_m*
const used_mem_get_used_mem() { return used_mem; }

used_m*
const used_mem_get_used_mem_by_id(const uint32 id) { return &used_mem[id]; }

uint32
used_mem_get_count() { return used_mem_count; }

int32
used_mem_get_tail() { return used_mem_tail; }

pageid_t
used_mem_insert(void* addr, const pageid_t num_pages)
{
    // calculate the index of the start of the page from pointer adresses
    const void* start_addr = mem_get_address();
    uint32 id = 0;
    if(addr == start_addr) id = 0;
    else id = (pageid_t) (addr - start_addr) / PAGE_SIZE;

    if(id >= START_USED_COUNT) {
        my_exit("ERROR: used_mem exceeds START_USED_COUNT", __FILE__, __LINE__);
    }

    used_mem[id].addr = addr;
    used_mem[id].id = id;
    used_mem[id].num_pages = num_pages;
    used_mem_count++;
    if(id < used_mem_count) used_mem_tail++;
    mem_log_print_used_mem_buf("insert", &used_mem[id]);
    return used_mem[id].id;
}


void
used_mem_remove(const pageid_t id)
{
    used_mem[id].addr = NULL;
    used_mem[id].num_pages = 0;
    used_mem_count--;

    if(id >= used_mem_tail)
        while(used_mem_tail >= INT_NULL && used_mem[used_mem_tail].addr == NULL)
            used_mem_tail--;


    mem_log_print_used_mem_buf("remove", &used_mem[id]);
}

/*
 * functionality
 */
void* const get_address(const pageid_t id) { return used_mem[id].addr; }

struct memhnd_t
mms_alloc(const pageid_t num_pages, const enum MEM_CHUNK_SELECTOR chunk_selector)
{
    // CHECK_MEM_NULL(mem, NO_MEMORY_MANAGER_INIT, "mms_alloc");

    // MEM_CHUNK_SELECTOR
    const memid_t chunk_id = chunk_selector_get_chunk(chunk_selector);

    void* addr = flist_get_next_free_page(num_pages, chunk_id);
    // mem_log_print_free_list(free_list_head);
    // print("%s allocated address:%x\n", RED, REDUCE_ADDR(addr, F6), CLEAR);

    // HERE: we cant have the pageid on an allocation
    // we need to get it from the free list.
    // that means the free list will have to store the page id
    // along side its pointer to the free mem page
    const pageid_t page_id = flist_get_pageid_magically((struct free_list*)addr);
    // mem_used_set_pageid((pageid_t) page_id, page_id);

    struct memhnd_t handle = {
        .chunk_id = chunk_id,
        .page_id = page_id,
        .num_pages = num_pages,
    };
    return handle;
}

bool
mms_release(const struct memhnd_t handle)
{
    // cache data
    // void* start_addr = used_mem[id].addr;
    // const pageid_t num_pages = used_mem[id].num_pages;
    // if(num_pages == 0) {
    //     mem_print_error(MEMORY_IS_IN_USE);
    //     return FALSE;
    //     // my_exit("mms_release", __FILE__, __LINE__);
    // }
    // used_mem_remove(id);
    //
    // const bool success = free_list_insert(id, num_pages);
    // mem_log_print_free_list(free_list_head);

    // return success;
}
