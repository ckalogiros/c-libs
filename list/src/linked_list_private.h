/*
 * Created by ckalo on 1/13/26.
 */
#ifndef LIST_LINKED_LIST_PRIVATE_H
#define LIST_LINKED_LIST_PRIVATE_H

#include "linked_list.h"

#define MAX_LIST_ELEMS_COUNT 16

/*
 * private functions
 */
sll_node_t* node_alloc();
void sll_release(const sll_node_t* node);
sll_node_t* node_alloc_renameme(sll_node_t* const head);

sll_node_t* find_byid_ret_prev(sll_node_t* node, const list_node_id id);

void handle_error(enum LIST_ERROR_CODE error_code);

#endif //LIST_LINKED_LIST_PRIVATE_H