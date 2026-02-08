/*
 * Created by ckalo on 2/8/26.
 */

#include "logger.h"
#include <stdlib.h>
#include "mem_structs.h"
#include "mem_test.h"
#include "sys_utils.h"


static struct mem* mem;
static struct mem_chunk* chunks_table;       // mem is devided into chunks
static struct mem_used* mem_used_table;      // table stores: mem used buffers per chunk
static struct free_list** flist_heads_table; // table stores:flist heads per chunk


void mem_init_chunks_table();
void mem_init_used_mem_table();
void mem_init_free_list_table();


void mem_init()
{
    const uint32 pagesize = PAGE_SIZE;
    const uint32 num_pages = 512;
    const uint32 alignment = PAGE_SIZE * 16;
    const uint32 self_storage_size = PAGE_SIZE;
    const uint32 memory_bound_check = PAGE_SIZE;

    const uint32 total_size = self_storage_size + (num_pages * pagesize) + memory_bound_check;

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
    void* off_bound_mem_start = addr+(self_storage_size + (num_pages * pagesize));
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

    uint16 val      = 0;
    uint16 sval     = 0;
    uint16 start_bit= 0;
    uint16 end_bit  = 0;

    for(uint32 chunk_id = 0; chunk_id < mem->chunks_count; chunk_id++) {
        for(uint32 page_id = 0; page_id < PAGES_PER_CHUNK; page_id++) {

            /* *****************************************
             * calc mask and set bitfield of chunks id */
            val = mem_used_table[page_id].ids;
            sval = chunk_id;
            start_bit = flag_ids.chunk_id_start_bit;
            end_bit = flag_ids.chunk_id_end_bit;
            uint32 chunk_id_mask = bits_calc_bitmask_from_start_end_bits(val, start_bit, end_bit);
            mem_used_table[page_id].ids = bitfield_set(val, sval, chunk_id_mask, start_bit);
            // print("[%u] chunk_id:%u mask:%x\n", page_id, bitfield_get(val, chunk_id_mask, start_bit), chunk_id_mask);



            /* *****************************************
             * calc mask and set bitfield of page id */
            val = mem_used_table[page_id].ids;
            sval = page_id;
            start_bit = flag_ids.page_id_start_bit;
            end_bit = flag_ids.page_id_end_bit;
            uint32 page_id_mask = bits_calc_bitmask_from_start_end_bits(val, start_bit, end_bit);

            mem_used_table[page_id].ids = bitfield_set(val, sval, page_id_mask, start_bit);

            // print-debug bitfield chunk_ids and page_ids
            // print("[%u] page_id:%u ", page_id, bitfield_get(val, page_id_mask, start_bit));
            // print("chunk_id:%u\n", bitfield_get(mem_used_table[page_id].ids, chunk_id_mask, flag_ids.chunk_id_start_bit));

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

        // assign to each fflist head the mem start of each chunk
        flist_heads_table[chunk_id] = (struct free_list*) page_ptr;
        struct free_list* flist_head = flist_heads_table[chunk_id];
        // print("%sflist head:%x%s\n", YELLOW, REDUCE_ADDR(flist_head, F5), CLEAR);

        for(uint16 i = 0; i < PAGES_PER_CHUNK; i++) {

            /* ***********************************************************
             * create new free list, for every page of a chunk, via the page pointer,
             * connect ->next to point at previous page hold by head,
             * finally set head to current page
             *  */
            struct free_list* flist = (struct free_list*) page_ptr;
            flist->next = flist_head;
            flist_head = flist;
            page_ptr += PAGE_SIZE;

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
        uint32 nbits = 6;
        uint32 start_bit = 0;
        uint32 end_bit = start_bit + nbits;
        uint32 mask = bits_calc_bitmask_from_start_end_bits(start_bit, start_bit, end_bit);

        print("%sresult mask%s\n", ry, CLEAR);
        bits_print_binary(mask, sizeof(mask));

        uint32 expected = 0b111111;
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
        uint32 nbits = 6;
        uint32 start_bit = 4;
        uint32 end_bit = start_bit + nbits;
        uint32 mask = bits_calc_bitmask_from_start_end_bits(start_bit, start_bit, end_bit);

        print("%sresult mask%s\n", ry, CLEAR);
        bits_print_binary(mask, sizeof(mask));

        uint32 expected = 0b1111110000;
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
        uint32 nbits = 17;
        uint32 start_bit = 11;
        uint32 end_bit = start_bit + nbits;
        uint32 mask = bits_calc_bitmask_from_start_end_bits(start_bit, start_bit, end_bit);

        print("%sresult mask%s\n", ry, CLEAR);
        bits_print_binary(mask, sizeof(mask));

        uint32 expected = 0b00001111111111111111100000000000;
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
        uint16 val      = 0b111111110000;
        uint16 sval     = 0b1010;
        uint16 mask     = 0b11110000;
        uint16 nbits    = 4;

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
        uint16 val      = 0b100011110000;
        uint16 sval     = 0b1010;
        uint16 mask     = 0b11110000;
        uint16 nbits    = 4;

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

void test_macr0_impl_bitfield_set()
{

    {
        print("-- TEST 1 \n   \'bitfield_set MACRO impl\'\n");
        uint16 val      = 0b111111110000;
        uint16 sval     = 0b1010;
        uint16 mask     = 0;
        uint16 start_bit= 4;
        uint16 end_bit  = 8;
        uint16 nbits    = 4;

        // expected_mask = 0b11110000;
        mask = bits_calc_bitmask_from_start_end_bits(mask, start_bit, end_bit);

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
        mask = bits_calc_bitmask_from_start_end_bits(mask, start_bit, end_bit);

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