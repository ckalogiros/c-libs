/*
 * Created by ckalo on 8/22/26.
 */
#ifndef MBUFFER_MBUFFER_INTERFACE_H
#define MBUFFER_MBUFFER_INTERFACE_H

#include "my_types.h"

typedef uint32 mbuf_t;

struct mbuffer {
    void*   data;
    mbuf_t* idxs;
    mbuf_t* ids;
    mbuf_t  capacity;           // total buffer elements count
    mbuf_t  count;              // used elements count
    uint16  typesize;           // size of the type of data
};

#endif //MBUFFER_MBUFFER_INTERFACE_H