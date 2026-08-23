/*
 * Created by ckalo on 8/22/26.
 */
#ifndef MBUFFER_MBUFFER_INTERNAL_H
#define MBUFFER_MBUFFER_INTERNAL_H
#include "mbuffer_interface.h"

void mbuffers_pool_init();
struct mbuffer* mbuffers_pool_alloc_mbuffer(void);

#endif //MBUFFER_MBUFFER_INTERNAL_H