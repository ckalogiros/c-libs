/*
 * Created by ckalo on 2/6/26.
 */
#ifndef HTML_GEN_HTML_STRUCTS_H
#define HTML_GEN_HTML_STRUCTS_H
#include "my_types.h"

struct html_node {
    struct html_node *child;
    struct html_node *sibling;
    uint32 uid; // unique id: for search
    uint32 group_id; // info for memory, for allocators, for buffer, ...
    uint32 tag_id; // div,p,body, ...
    uint64 class_bitfield; // div,p,body, ...
    const char* name; // todo: store to mem properly
};

static uint32 uuid = 0;

enum TAG_IDS {
    NULL_TAG = 0,
    HTML_TAG = 1,
    HEAD_TAG,
    BODY_TAG,
    STYLE_TAG,

    DIV_TAG,
    P_TAG,
    SPAN_TAG,

    H1_TAG,
    H2_TAG,
    H3_TAG,

    A_TAG,

    // todo: button tags etc.
    // todo: custom tags like  color tags
};

enum CLASS_IDS {
    CLASS_NULL = 0x0,
    CLASS_1 = BIT_MASK_FROM_UINT(1),
    CLASS_2 = BIT_MASK_FROM_UINT(2),
    CLASS_3 = BIT_MASK_FROM_UINT(3),
    // ...
};
enum GROUP_IDS {
    GROUP_NULL = 0,
    GROUP_MEMORY,
    GROUP_2,
    GROUP_3,
    // ...
};

#endif //HTML_GEN_HTML_STRUCTS_H