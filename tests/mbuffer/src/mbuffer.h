/*
 * Created by ckalo on 8/22/26.
 */
#ifndef MBUFFER_MBUFFER_H
#define MBUFFER_MBUFFER_H
#include "mbuffer_interface.h"





void mbuffer_init(void);

struct mbuffer* mbuffer_create(mbuf_t capacity);

void mbuffer_destroy(void);

void mbuffer_deinit(void);

#endif //MBUFFER_MBUFFER_H
