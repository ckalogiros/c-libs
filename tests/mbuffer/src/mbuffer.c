/*
 * Created by ckalo on 8/22/26.
 */

#include "mbuffer.h"

#include "mbuffer_internal.h"


struct mbuffer* mbuffer_create(mbuf_t capacity)
{
    struct mbuffer* mbuffer = mbuffers_pool_alloc_mbuffer();
    mbuffer->data = NULL;
    mbuffer->idxs = NULL;
    mbuffer->ids  = NULL;
    mbuffer->capacity = capacity;
    mbuffer->count = 0;
    mbuffer->typesize = 0;

    return mbuffer;
}

void mbuffer_destroy(void){}

void mbuffer_deinit(void){}