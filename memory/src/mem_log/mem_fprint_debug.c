/*
 * Created by ckalo on 8/24/26.
 */

#include "mem_fprint_debug.h"
#include "file_sys_interface.h"
#include "logger.h"
#include "mem_man_internals.h"
#include "string_format.h"


/***************************************************************************************************
 * FOR DEBUG */
/* debug file for mem_block->freelist  */
struct fs_t* fs_flist = NULL;
/* debug file for mem_block  */
struct fs_t* fs_mem = NULL;
/* debug file for mem_block poison data  */
struct fs_t* fs_poison = NULL;
/* FOR DEBUG
 **************************************************************************************************/

/* debug-fprint the memory pool buffer */
void mem_fprint_append_mem_pool(struct mem_block* mem_pool, const uint32 capacity,
                                       const uint32 size, const uint32 allocated_size) {

    if(fs_mem == NULL) fs_mem = fs_create("Debug/memory/mem.txt");

    for(uint32 i = 0; i < capacity; i++) {

        struct mem_block* mem_block = &mem_pool[i];
        print("pool.block[%u]:%p\n", i, mem_block);
        if(mem_block != NULL) {

            fs_append(fs_mem,
                "[%u]\n"
                "mem creator:       %s\n"
                "address:           %08x %08x\n"
                "flist:             %08x %08x\n"
                "size:              %u\n"
                "allocated_size:    %u\n"
                "-------------------------------\n",
                i,
                mem_block->name,
                (uint64) mem_block->start >> 32, (uint64) mem_block->start & 0xFFFFFFFF,
                (uint64) mem_block->flist >> 32, (uint64) mem_block->flist & 0xFFFFFFFF,
                size,
                allocated_size
            );
        }
    }
}

void mem_fprint_append_mem_block_to_file(struct fs_t* fs, const memid_t memid) {

    struct mem_block* mem_block = mem_pool_get_block(memid);

    fs_append(fs,
        "\n"
        "###################\n"
        "# mem creator:       %16s\n"
        "# mem id:            %u\n"
        "# address:           %08x %08x\n"
        "# flist:             %08x %08x\n"
        "# size:              %u\n"
        "# data_size:         %u\n"
        "# num_pages:         %u\n"
        "# poison_data_size:  %u\n"
        "###################\n",
        mem_block->name,
        memid,
        (uint32) ((uint64)mem_block->start >> 32), (uint32) ((uint64)mem_block->start & 0xFFFFFFFF),
        (uint32) ((uint64)mem_block->flist >> 32), (uint32) ((uint64)mem_block->flist & 0xFFFFFFFF),
        mem_block->size,
        mem_block->data_size,
        mem_block->num_pages,
        mem_block->poison_data_size
    );

    fs_append_mem(fs, mem_block->start, mem_block->data_size, "memory:block->data");

}


void mem_fprint_append_mem_block(const memid_t memid) {

    if(fs_mem == NULL) fs_mem = fs_create("Debug/memory/mem.txt");
    if(fs_flist == NULL) fs_flist = fs_create("Debug/memory/mem-flist.txt");

    struct mem_block* mem_block = mem_pool_get_block(memid);

    fs_append(fs_mem,
        "\n"
        "###################\n"
        "# mem creator:       %16s\n"
        "# mem id:            %u\n"
        "# address:           %08x %08x\n"
        "# flist:             %08x %08x\n"
        "# size:              %u\n"
        "# data_size:         %u\n"
        "# num_pages:         %u\n"
        "# poison_data_size:  %u\n"
        "###################\n",
        mem_block->name,
        memid,
        (uint32) ((uint64)mem_block->start >> 32), (uint32) ((uint64)mem_block->start & 0xFFFFFFFF),
        (uint32) ((uint64)mem_block->flist >> 32), (uint32) ((uint64)mem_block->flist & 0xFFFFFFFF),
        mem_block->size,
        mem_block->data_size,
        mem_block->num_pages,
        mem_block->poison_data_size
    );

    fs_append_mem(fs_mem, mem_block->start, mem_block->data_size, "memory:block->data");


    /* write free list */
    fs_append(fs_flist,
        "mem id:            %u\n"
        "mem creator:       %s\n"
        "mem start:         %08x %08x\n",
        memid, mem_block->name,
        (uint32) ((uint64) mem_block->start >> 32), (uint32) ((uint64) mem_block->start & 0xFFFFFFFF));

    struct flist* flist = mem_block->flist;
    uint32 cnt = 0;
    while(flist) {
        fs_append(fs_flist,
             "[%3u] flist: %08x %08x"
             " -->  %08x %08x\n"
             "",
             cnt++,
             (uint32) ((uint64)flist >> 32), (uint32) ((uint64)flist & 0xFFFFFFFF),
             (uint32) ((uint64)flist->next >> 32), (uint32) ((uint64)flist->next & 0xFFFFFFFF)
        );

        flist = flist->next;
    }

}


/* debug-fprint the memory pool buffer */
void mem_fprint_append_mem_block_poison(const memid_t memid) {

    if(fs_poison == NULL) fs_poison = fs_create("Debug/memory/block_poison.txt");

    struct mem_block* mem_block = mem_pool_get_block(memid);
    if(mem_block->start != NULL) {

        print("POISON: memid:%u at %p\n", memid, mem_block);
        /// calc poison start
        char* poison_ptr = mem_block->start+mem_block->data_size;
        const uint32 bufsize = BLOCK_POISON_DATA_SIZE-sizeof(struct flist);
        char buf[BLOCK_POISON_DATA_SIZE-sizeof(struct flist)];

        /// add the size of the last freelist which is always NULL
        char* poison_data = poison_ptr+sizeof(struct flist);
        char* ptr = &buf[0];
        string_format(&ptr, bufsize, "%s", poison_data);

        /// create a printable char array for the NULL freelist
        char flist_buf[sizeof(struct flist)];
        char* byte = (char*)&poison_ptr[0];
        for(int k = 0; k < 8; k++)
            flist_buf[k] = byte[k]+0x30;
        flist_buf[7] = '\0';  // Null-terminate in case of corrupted memory


        print("8 byte NULL free list: %s\n", flist_buf);
        print("POISON: %s\n", buf);
        fs_append(fs_poison,
            "mem id    :        %d\n"
            "mem creator:       %s\n"
            "address:           %08x %08x\n"
            "size:              %u bytes\n"
            "data:              %s %s\n"
            "-------------------------------\n",
            memid,
            mem_block->name,
            ((uint64)poison_ptr >> 32), (uint32) ((uint64)poison_ptr & 0xFFFFFFFF),
            BLOCK_POISON_DATA_SIZE,
            flist_buf, buf
        );
    }
}

void mem_fprint_append_all_mem_block_poison(struct mem_block* mem_pool, const uint32 capacity) {

    if(fs_poison == NULL) fs_poison = fs_create("Debug/memory/block_poison.txt");

    for(uint32 i = 0; i < capacity; i++) {

        struct mem_block* mem_block = &mem_pool[i];
        if(mem_block->start != NULL) {

            print("POISON: block[%u] at %p\n", i, mem_block);
            /// calc poison start
            char* poison_ptr = mem_block->start+mem_block->data_size;
            const uint32 bufsize = BLOCK_POISON_DATA_SIZE-sizeof(struct flist);
            char buf[BLOCK_POISON_DATA_SIZE-sizeof(struct flist)];

            /// add the size of the last freelist which is always NULL
            char* poison_data = poison_ptr+sizeof(struct flist);
            char* ptr = &buf[0];
            string_format(&ptr, bufsize, "%s", poison_data);

            /// create a printable char array for the NULL freelist
            char flist_buf[sizeof(struct flist)];
            char* byte = (char*)&poison_ptr[0];
            for(int k = 0; k < 8; k++)
                flist_buf[k] = byte[k]+0x30;
            flist_buf[7] = '\0';  // Null-terminate in case of corrupted memory


            print("8 byte NULL free list: %s\n", flist_buf);
            print("POISON: %s\n", buf);
            fs_append(fs_poison,
                "mem block id:      [%u]\n"
                "mem creator:       %s\n"
                "address:           %08x %08x\n"
                "size:              %u bytes\n"
                "data:              %s %s\n"
                "-------------------------------\n",
                i,
                mem_block->name,
                (uint64) poison_ptr >> 32, (uint64) poison_ptr & 0xFFFFFFFF,
                BLOCK_POISON_DATA_SIZE,
                flist_buf, buf
            );
        }
    }
}

