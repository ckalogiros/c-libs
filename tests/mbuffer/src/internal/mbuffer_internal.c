/*
 * Created by ckalo on 8/22/26.
 */

#include "mbuffer_internal.h"
#include "mbuffer_interface.h"
#include "mem_man_interface.h"
#include <assert.h>
#include <stdlib.h>

#define MBUFFER_MEM_BLOCK_ALIGNMENT 1024
#define MBUFFERS_POOL_COUNT MBUFFER_MEM_BLOCK_ALIGNMENT/sizeof(struct mbuffer)

// uint32 mbuffers_capacity = MBUFFER_MEM_BLOCK_ALIGNMENT/sizeof(struct mbuffer);
// uint32 mbuffers_count = 0;
struct mbuffer* mbuffers_pool = NULL;

void mbuffers_pool_init()
{
    assert(mbuffers_pool==NULL); /// make sure is not been initialized more than once

    const uint32 alignment = MBUFFER_MEM_BLOCK_ALIGNMENT;
    const uint32 capacity = MBUFFERS_POOL_COUNT;
    const uint32 num_pages = ALIGN_;
    const memid_t data_memid =
        memman_allocate_block(num_pages, alignment, "struct mbuffer* mbuffers_pool");
    mbuffers_pool =
        aligned_alloc(alignment, capacity*sizeof(struct mbuffer));
    assert(mbuffers_pool!=NULL);

    /// struct initialization
    mbuffers_pool->typesize = sizeof(struct mbuffer);
    mbuffers_pool->capacity = capacity;
    mbuffers_pool->count    = 0;
    mbuffers_pool->data     = NULL;
    mbuffers_pool->idxs     = NULL;
    mbuffers_pool->ids      = NULL;
}

struct mbuffer* mbuffers_pool_alloc_mbuffer(void)
{
    if(mbuffers_pool==NULL)
        mbuffers_pool_init();

    const mbuf_t idx = mbuffers_pool->count;
    assert(idx<mbuffers_pool->capacity); /// TODO: implement realloc
    mbuffers_pool[idx].count++;
    return &mbuffers_pool[idx];
}

void mbuffers_pool_free(const mbuf_t idx)
{
    // mbuffers_pool->data     = NULL;
    // mbuffers_pool->idxs[]     = NULL;
    // mbuffers_pool->ids[]    = NULL;
    // mbuffers_pool->count--;
}