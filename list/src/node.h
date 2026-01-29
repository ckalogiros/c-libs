/*
 * Created by ckalo on 1/12/26.
 */
#ifndef LIST_NODE_H
#define LIST_NODE_H

#include "my_types.h"

typedef uint16 node_size_t;

// generic node struct
typedef struct node_gt {
    void* data;
    struct node_gt* next;
} node_gt;

#endif //LIST_NODE_H