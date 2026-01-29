/*
 * Created by ckalo on 1/13/26.
 */

#include "linked_list_private.h"
#include "sys_utils.h"
#include "logger.h"
#include "mem_man.h"
// #include "test_allocator.h"


sll_node_t* node_alloc()
{
    const pageid_t page_id = allocator_test_alloc(sizeof(sll_node_t) * MAX_LIST_ELEMS_COUNT);
    sll_node_t* node = (sll_node_t*)Temp_get_address_by_id(page_id);
    if (node == NULL) my_exit("node pointer is null", __FILE__, __LINE__);
    return node;
}

sll_node_t* node_alloc_renameme(sll_node_t* const head)
{
    // loop through the allocated memory as if it where a buffer
    for (uint32 i = 0; i < MAX_LIST_ELEMS_COUNT; i++)
    {
        sll_node_t* next = head + i;
        print("    address:%x\n", next);
        if (next->data == NULL)
        {
            print("%saddress:%x%s\n", GREEN, next, CLEAR);
            return next;
        }
    }
    // case list is full
    // todo: implement realloc
    return NULL;
}

void sll_release(const sll_node_t* node)
{
    if (node == NULL) {
        handle_error(NODE_NULL);
        return;
    }

    node = NULL;
}

sll_node_t* find_byid_ret_prev(sll_node_t* node, const list_node_id id)
{
    sll_node_t* prev = node;
    sll_node_t* curr = node->next;
    while (curr != NULL) {

        if (curr->id == id) { return prev; }
        prev = curr;
        curr = curr->next;
    }
    return NULL;
}

void handle_error(const enum LIST_ERROR_CODE error_code)
{
    print("ERROR. code:%x\n", error_code);
}