/*
 * Created by ckalo on 12/30/25.
 *
 * linked list with a buffer memory allocator
 *
 * Automatic memory allocations for growing the list
 * What is guaranteed is the locality of the elements in the list
 * What is not guaranteed is that list's elements in real memory be in the order of the links
 * todo: Use the sort function to sort buffer in the order of the list links
 */


#ifndef LIST_LINKED_LIST_H
#define LIST_LINKED_LIST_H


#include "my_types.h"

typedef uint16 node_size_t;
typedef uint16 list_node_id; // id type for linked list nodes


enum LIST_ERROR_CODE {
    RESERVED  = BIT_MASK_FROM_UINT(0),
    NOT_FOUND = BIT_MASK_FROM_UINT(1),
    NODE_NULL = BIT_MASK_FROM_UINT(3),
    OP_INSERT = BIT_MASK_FROM_UINT(10),
    OP_DELETE = BIT_MASK_FROM_UINT(11),
};

// generic node struct
typedef struct sll_node_t {
    void* data;
    struct sll_node_t* next;
    list_node_id id;
} sll_node_t;

//typedef struct single_linked_list {
//    sll_node_t* head;
//    node_size_t size;
//} sll_t;


sll_node_t *sll_create_list(void* const data);
list_node_id sll_insert(sll_node_t* const head, void* const data);
void sll_delete_byid(sll_node_t** head, const list_node_id id);

#endif //LIST_LINKED_LIST_H


